#pragma once

#include "PluginProcessor.h"

// Forward declarations
class juce::AudioProcessorValueTreeState;

namespace audio_plugin {

    // ============================================================================
    // LAYOUT CONSTANTS
    // ============================================================================
    constexpr int WINDOW_BORDER_MARGIN = 10;
    constexpr int LABEL_WIDTH = 100;
    constexpr int COMPONENT_HEIGHT = 30;
    constexpr int LABEL_HEIGHT = 20;
    constexpr int HEADER_HEIGHT = 50;

    class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
    public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;


	juce::Slider mSliderDepth;
	juce::Slider mSliderNumVoices;
	juce::Label mLabelNumVoices;
	
	// Parameter attachments
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mNumVoicesAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
    };
    
} // namespace audio_plugin