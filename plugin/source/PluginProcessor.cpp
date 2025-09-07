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
        
        // ============================================================================
        // MAIN CONTROLS PARAMETERS
        // ============================================================================
        
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
        
        // Rate parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "rate",                // parameterID
            "Rate",                // parameter name
            juce::NormalisableRange<float>(0.1f, 2.0f, 0.01f), // range
            1.0f,                  // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("Hz")
        ));
        
        // Depth parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "depth",               // parameterID
            "Depth",               // parameter name
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), // range
            0.5f,                  // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("")
        ));
        
        // Mix parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "mix",                 // parameterID
            "Mix",                 // parameter name
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), // range
            0.7f,                  // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("")
        ));
        
        // Base Delay parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "baseDelay",           // parameterID
            "Base Delay",          // parameter name
            juce::NormalisableRange<float>(10.0f, 100.0f, 0.1f), // range
            30.0f,                 // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("ms")
        ));
        
        // Voice Attenuation parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "voiceAttenuation",    // parameterID
            "Voice Attenuation",   // parameter name
            juce::NormalisableRange<float>(-6.0f, 0.0f, 0.1f), // range
            0.0f,                  // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("dB")
        ));
        
        // Master Voice Level parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "masterVoiceLevel",    // parameterID
            "Master Voice Level",  // parameter name
            juce::NormalisableRange<float>(-20.0f, 6.0f, 0.1f), // range
            -3.0f,                 // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("dB")
        ));
        
        // Enabled parameter
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            "enabled",             // parameterID
            "Enabled",             // parameter name
            true,                  // default value
            juce::AudioParameterBoolAttributes()
        ));
        
        // ============================================================================
        // STEREO SECTION PARAMETERS
        // ============================================================================
        
        // Stereo Mode parameter
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            "stereoMode",          // parameterID
            "Stereo Mode",         // parameter name
            juce::StringArray{"Mono", "Stereo", "Mid-Side"}, // choices
            0,                     // default index (Mono)
            juce::AudioParameterChoiceAttributes()
        ));
        
        // Stereo Spread parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "stereoSpread",        // parameterID
            "Stereo Spread",       // parameter name
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), // range
            0.5f,                  // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("")
        ));
        
        // Mid Enabled parameter
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            "midEnabled",          // parameterID
            "Mid Enabled",         // parameter name
            true,                  // default value
            juce::AudioParameterBoolAttributes()
        ));
        
        // Side Enabled parameter
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            "sideEnabled",         // parameterID
            "Side Enabled",        // parameter name
            true,                  // default value
            juce::AudioParameterBoolAttributes()
        ));
        
        // Side Gain parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "sideGain",            // parameterID
            "Side Gain",           // parameter name
            juce::NormalisableRange<float>(-20.0f, 20.0f, 0.1f), // range
            0.0f,                  // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("dB")
        ));
        
        // ============================================================================
        // FILTER SECTION PARAMETERS
        // ============================================================================
        
        // LPF Enabled parameter
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            "lpfEnabled",          // parameterID
            "LPF Enabled",         // parameter name
            false,                 // default value
            juce::AudioParameterBoolAttributes()
        ));
        
        // LPF Cutoff parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "lpfCutoff",           // parameterID
            "LPF Cutoff",          // parameter name
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f), // range
            20000.0f,              // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("Hz")
        ));
        
        // HPF Enabled parameter
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            "hpfEnabled",          // parameterID
            "HPF Enabled",         // parameter name
            false,                 // default value
            juce::AudioParameterBoolAttributes()
        ));
        
        // HPF Cutoff parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "hpfCutoff",           // parameterID
            "HPF Cutoff",          // parameter name
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f), // range
            20.0f,                 // default value
            juce::AudioParameterFloatAttributes()
                .withLabel("Hz")
        ));
        
        return { params.begin(), params.end() };
    }
    
    // ============================================================================
    // PARAMETER UPDATE METHODS
    // ============================================================================
    
    void AudioPluginAudioProcessor::updateChorusParameters() {
        // ============================================================================
        // MAIN CONTROLS PARAMETERS
        // ============================================================================
        
        // Update numVoices parameter
        auto* numVoicesParam = mParameterTree.getRawParameterValue("numVoices");
        if (numVoicesParam != nullptr) {
            int numVoices = static_cast<int>(numVoicesParam->load());
            mChorusModule.setNumVoices(numVoices);
        }
        
        // Update rate parameter
        auto* rateParam = mParameterTree.getRawParameterValue("rate");
        if (rateParam != nullptr) {
            float rate = rateParam->load();
            mChorusModule.setRate(rate);
        }
        
        // Update depth parameter
        auto* depthParam = mParameterTree.getRawParameterValue("depth");
        if (depthParam != nullptr) {
            float depth = depthParam->load();
            mChorusModule.setDepth(depth);
        }
        
        // Update mix parameter
        auto* mixParam = mParameterTree.getRawParameterValue("mix");
        if (mixParam != nullptr) {
            float mix = mixParam->load();
            mChorusModule.setMix(mix);
        }
        
        // Update baseDelay parameter
        auto* baseDelayParam = mParameterTree.getRawParameterValue("baseDelay");
        if (baseDelayParam != nullptr) {
            float baseDelay = baseDelayParam->load();
            mChorusModule.setBaseDelay(baseDelay);
        }
        
        // Update voiceAttenuation parameter
        auto* voiceAttenuationParam = mParameterTree.getRawParameterValue("voiceAttenuation");
        if (voiceAttenuationParam != nullptr) {
            float voiceAttenuation = voiceAttenuationParam->load();
            mChorusModule.setVoiceAttenuation(voiceAttenuation);
        }
        
        // Update masterVoiceLevel parameter
        auto* masterVoiceLevelParam = mParameterTree.getRawParameterValue("masterVoiceLevel");
        if (masterVoiceLevelParam != nullptr) {
            float masterVoiceLevel = masterVoiceLevelParam->load();
            mChorusModule.setMasterVoiceLevel(masterVoiceLevel);
        }
        
        // Update enabled parameter
        auto* enabledParam = mParameterTree.getRawParameterValue("enabled");
        if (enabledParam != nullptr) {
            bool enabled = enabledParam->load() > 0.5f;
            mChorusModule.setEnabled(enabled);
        }
        
        // ============================================================================
        // STEREO SECTION PARAMETERS
        // ============================================================================
        
        // Update stereoMode parameter
        auto* stereoModeParam = mParameterTree.getRawParameterValue("stereoMode");
        if (stereoModeParam != nullptr) {
            int stereoMode = static_cast<int>(stereoModeParam->load());
                    // Convert to WizardCore::Chorus::PanningMode enum
        WizardCore::Chorus::PanningMode mode = static_cast<WizardCore::Chorus::PanningMode>(stereoMode);
            mChorusModule.setStereoMode(mode);
        }
        
        // Update stereoSpread parameter
        auto* stereoSpreadParam = mParameterTree.getRawParameterValue("stereoSpread");
        if (stereoSpreadParam != nullptr) {
            float stereoSpread = stereoSpreadParam->load();
            mChorusModule.setStereoSpread(stereoSpread);
        }
        
        // Update midEnabled parameter
        auto* midEnabledParam = mParameterTree.getRawParameterValue("midEnabled");
        if (midEnabledParam != nullptr) {
            bool midEnabled = midEnabledParam->load() > 0.5f;
            mChorusModule.setMidEnabled(midEnabled);
        }
        
        // Update sideEnabled parameter
        auto* sideEnabledParam = mParameterTree.getRawParameterValue("sideEnabled");
        if (sideEnabledParam != nullptr) {
            bool sideEnabled = sideEnabledParam->load() > 0.5f;
            mChorusModule.setSideEnabled(sideEnabled);
        }
        
        // Update sideGain parameter
        auto* sideGainParam = mParameterTree.getRawParameterValue("sideGain");
        if (sideGainParam != nullptr) {
            float sideGain = sideGainParam->load();
            mChorusModule.setSideGain(sideGain);
        }
        
        // ============================================================================
        // FILTER SECTION PARAMETERS
        // ============================================================================
        
        // Update lpfEnabled parameter
        auto* lpfEnabledParam = mParameterTree.getRawParameterValue("lpfEnabled");
        if (lpfEnabledParam != nullptr) {
            bool lpfEnabled = lpfEnabledParam->load() > 0.5f;
            mChorusModule.setLPFEnabled(lpfEnabled);
        }
        
        // Update lpfCutoff parameter
        auto* lpfCutoffParam = mParameterTree.getRawParameterValue("lpfCutoff");
        if (lpfCutoffParam != nullptr) {
            float lpfCutoff = lpfCutoffParam->load();
            mChorusModule.setLPFCutoff(lpfCutoff);
        }
        
        // Update hpfEnabled parameter
        auto* hpfEnabledParam = mParameterTree.getRawParameterValue("hpfEnabled");
        if (hpfEnabledParam != nullptr) {
            bool hpfEnabled = hpfEnabledParam->load() > 0.5f;
            mChorusModule.setHPFEnabled(hpfEnabled);
        }
        
        // Update hpfCutoff parameter
        auto* hpfCutoffParam = mParameterTree.getRawParameterValue("hpfCutoff");
        if (hpfCutoffParam != nullptr) {
            float hpfCutoff = hpfCutoffParam->load();
            mChorusModule.setHPFCutoff(hpfCutoff);
        }
    }

} // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}