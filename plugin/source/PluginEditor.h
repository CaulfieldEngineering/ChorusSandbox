#pragma once

#include "PluginProcessor.h"

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

    // ============================================================================
    // UI COMPONENT SETUP METHODS
    // ============================================================================
    
    // Section Headers and Groups
    void makeSectionHeaders();
    void makeSectionGroups();
    
    // Main Controls Section
    void makeEnabledControl();
    void makeNumVoicesControl();
    void makeVoiceAttenuationControl();
    void makeMasterVoiceLevelControl();
    void makeMixControl();
    void makeRateControl();
    void makeDepthControl();
    void makeBaseDelayControl();
    
    // Stereo Section
    void makeStereoModeControl();
    void makeStereoSpreadControl();
    void makeMidEnabledControl();
    void makeSideEnabledControl();
    void makeSideGainControl();
    
    // Filter Section
    void makeLPFEnabledControl();
    void makeLPFCutoffControl();
    void makeHPFEnabledControl();
    void makeHPFCutoffControl();

    // ============================================================================
    // UI COMPONENTS - SECTION HEADERS AND GROUPS
    // ============================================================================
    juce::Label mLabelGlobalControlsHeader;
    juce::Label mLabelStereoControlsHeader;
    juce::Label mLabelFilteringHeader;
    
    juce::GroupComponent mGroupGlobalControls;
    juce::GroupComponent mGroupStereoControls;
    juce::GroupComponent mGroupFiltering;

    // ============================================================================
    // UI COMPONENTS - MAIN CONTROLS
    // ============================================================================
    juce::ToggleButton mToggleEnabled;
    juce::Label mLabelEnabled;
    
    juce::Slider mSliderNumVoices;
    juce::Label mLabelNumVoices;
    
    juce::Slider mSliderVoiceAttenuation;
    juce::Label mLabelVoiceAttenuation;
    
    juce::Slider mSliderMasterVoiceLevel;
    juce::Label mLabelMasterVoiceLevel;
    
    juce::Slider mSliderMix;
    juce::Label mLabelMix;
    
    juce::Slider mSliderRate;
    juce::Label mLabelRate;
    
    juce::Slider mSliderDepth;
    juce::Label mLabelDepth;
    
    juce::Slider mSliderBaseDelay;
    juce::Label mLabelBaseDelay;

    // ============================================================================
    // UI COMPONENTS - STEREO SECTION
    // ============================================================================
    juce::ComboBox mComboStereoMode;
    juce::Label mLabelStereoMode;
    
    juce::Slider mSliderStereoSpread;
    juce::Label mLabelStereoSpread;
    
    juce::ToggleButton mToggleMidEnabled;
    juce::Label mLabelMidEnabled;
    
    juce::ToggleButton mToggleSideEnabled;
    juce::Label mLabelSideEnabled;
    
    juce::Slider mSliderSideGain;
    juce::Label mLabelSideGain;

    // ============================================================================
    // UI COMPONENTS - FILTER SECTION
    // ============================================================================
    juce::ToggleButton mToggleLPFEnabled;
    juce::Label mLabelLPFEnabled;
    
    juce::Slider mSliderLPFCutoff;
    juce::Label mLabelLPFCutoff;
    
    juce::ToggleButton mToggleHPFEnabled;
    juce::Label mLabelHPFEnabled;
    
    juce::Slider mSliderHPFCutoff;
    juce::Label mLabelHPFCutoff;

    // ============================================================================
    // PARAMETER ATTACHMENTS
    // ============================================================================
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mNumVoicesAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mVoiceAttenuationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mMasterVoiceLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mBaseDelayAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mStereoModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mStereoSpreadAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mMidEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mSideEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSideGainAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mLPFEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mLPFCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mHPFEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mHPFCutoffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
    };
    
} // namespace audio_plugin