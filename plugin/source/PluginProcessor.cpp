#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace audio_plugin {
    AudioPluginAudioProcessor::AudioPluginAudioProcessor()
        : AudioProcessor(
            BusesProperties()
    #if !JucePlugin_IsMidiEffect
    #if !JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
    #endif
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    #endif
        ),
        mParameterTree(*this, nullptr, "Parameters", createParameterLayout()) {
    }

    AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

    const juce::String AudioPluginAudioProcessor::getName() const {
    return JucePlugin_Name;
    }

    bool AudioPluginAudioProcessor::acceptsMidi() const {
    #if JucePlugin_WantsMidiInput
    return true;
    #else
    return false;
    #endif
    }

    bool AudioPluginAudioProcessor::producesMidi() const {
    #if JucePlugin_ProducesMidiOutput
    return true;
    #else
    return false;
    #endif
    }

    bool AudioPluginAudioProcessor::isMidiEffect() const {
    #if JucePlugin_IsMidiEffect
    return true;
    #else
    return false;
    #endif
    }

    double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

    int AudioPluginAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
                // programs, so this should be at least 1, even if you're not really
                // implementing programs.
    }

    int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

    void AudioPluginAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
    }

    const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
    }

    void AudioPluginAudioProcessor::changeProgramName(int index,
                                                    const juce::String &newName) {
    juce::ignoreUnused(index, newName);
    }

    void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                                int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    
    // Prepare the chorus module
    mChorusModule.prepare(sampleRate, getTotalNumInputChannels());
    }

    void AudioPluginAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
    // Clear the chorus module buffers
    mChorusModule.clear();
    }

    bool AudioPluginAudioProcessor::isBusesLayoutSupported(
        const BusesLayout &layouts) const {
    #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
    #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
    #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    #endif

    return true;
    #endif
    }

    void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                                juce::MidiBuffer &midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update chorus parameters from the parameter tree
    updateChorusParameters();
    
    // Process audio through the chorus module
    mChorusModule.processBlock(buffer);
    }

    bool AudioPluginAudioProcessor::hasEditor() const {
    	return true; // (change this to false if you choose to not supply an editor)
    }

    juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor() {
    	 return new AudioPluginAudioProcessorEditor(*this);
    	//return new juce::GenericAudioProcessorEditor(*this);
    }

    void AudioPluginAudioProcessor::getStateInformation(
        juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Save the current parameter state
    auto state = mParameterTree.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
    }

    void AudioPluginAudioProcessor::setStateInformation(const void *data,
                                                        int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    
    // Restore the parameter state
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(mParameterTree.state.getType())) {
            mParameterTree.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
    }
    // ============================================================================
    // PARAMETER LAYOUT CREATION
    // ============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout() {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
        
        // Num. Voices parameter
        params.push_back(std::make_unique<juce::AudioParameterInt>(
            "numVoices",           // parameterID
            "Num. Voices",         // parameter name
            1,                     // minimum value
            10,                    // maximum value
            2,                     // default value
            juce::AudioParameterIntAttributes()
                .withLabel("voices")
        ));
        
        return { params.begin(), params.end() };
    }
    
    // ============================================================================
    // PARAMETER UPDATE METHODS
    // ============================================================================
    
    void AudioPluginAudioProcessor::updateChorusParameters() {
        // Update numVoices parameter
        auto* numVoicesParam = mParameterTree.getRawParameterValue("numVoices");
        if (numVoicesParam != nullptr) {
            int numVoices = static_cast<int>(numVoicesParam->load());
            mChorusModule.setNumVoices(numVoices);
        }
        
        // TODO: Add more parameter updates here as we add them
        // - Rate
        // - Depth  
        // - Mix
        // - Base Delay
        // - etc.
    }

} // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}