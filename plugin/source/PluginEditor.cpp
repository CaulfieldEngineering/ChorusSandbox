#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace audio_plugin {
    AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
        AudioPluginAudioProcessor &p)
        : AudioProcessorEditor(&p), processorRef(p) {
    juce::ignoreUnused(processorRef);
        // Make sure that before the constructor has finished, you've set the
        // editor's size to whatever you need it to be.
        setSize(400, 300);
        
            // Configure Num. Voices slider
    mSliderNumVoices.setSliderStyle(juce::Slider::LinearHorizontal);
    mSliderNumVoices.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    mSliderNumVoices.setRange(1, 10, 1);
    mSliderNumVoices.setValue(2);
    mSliderNumVoices.setDoubleClickReturnValue(true, 2);
    
    // Set text box text colour to black
    mSliderNumVoices.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
            // Configure label
    mLabelNumVoices.setText("Num. Voices", juce::dontSendNotification);
    mLabelNumVoices.setJustificationType(juce::Justification::centred);
    mLabelNumVoices.setColour(juce::Label::textColourId, juce::Colours::black);
    mLabelNumVoices.attachToComponent(&mSliderNumVoices, true);
        
            // Add components to editor
    addAndMakeVisible(mSliderNumVoices);
    addAndMakeVisible(mLabelNumVoices);
    
    // Create parameter attachment for Num. Voices slider
    mNumVoicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getParameterTree(), "numVoices", mSliderNumVoices);
    }

    AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

    void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
        g.fillAll(juce::Colours::beige);

        // Draw the title
        g.setColour(juce::Colours::black);
        g.setFont(juce::Font(32.0f).boldened());
        g.drawFittedText("Chorus Sandbox", 0, 0, getWidth(), HEADER_HEIGHT, juce::Justification::centred, 1);

    }

    void AudioPluginAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // Position Num. Voices slider below the header
    // Leave space on the left for the label
    int sliderX = WINDOW_BORDER_MARGIN + LABEL_WIDTH;
    int sliderY = HEADER_HEIGHT + WINDOW_BORDER_MARGIN;
    int sliderWidth = getWidth() - (WINDOW_BORDER_MARGIN * 2) - LABEL_WIDTH;
    
    mSliderNumVoices.setBounds(sliderX, sliderY, sliderWidth, COMPONENT_HEIGHT);
    }
} // namespace audio_plugin