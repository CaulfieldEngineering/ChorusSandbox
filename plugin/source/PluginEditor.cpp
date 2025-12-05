#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace audio_plugin {
    AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
        AudioPluginAudioProcessor &p)
        : AudioProcessorEditor(&p), processorRef(p) {
    juce::ignoreUnused(processorRef);
        // Make sure that before the constructor has finished, you've set the
        // editor's size to whatever you need it to be.
        setSize(600, 1025);
    setResizable(true, true);
        
        // Setup UI components
        makeSectionHeaders();
        makeSectionGroups();
        makeEnabledControl();
        makeNumVoicesControl();
        makeVoiceAttenuationControl();
        makeMasterVoiceLevelControl();
        makeMixControl();
        makeRateControl();
        makeDepthControl();
        makeBaseDelayControl();
        
        makeStereoModeControl();
        makeStereoSpreadControl();
        makeMidEnabledControl();
        makeSideEnabledControl();
        makeSideGainControl();
        


        
        makeLPFEnabledControl();
        makeLPFCutoffControl();
        makeHPFEnabledControl();
        makeHPFCutoffControl();
    }

    AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

    // ============================================================================
    // UI COMPONENT SETUP METHODS - SECTION HEADERS
    // ============================================================================
    
    void AudioPluginAudioProcessorEditor::makeSectionHeaders() {
        // Configure Global Controls header
        mLabelGlobalControlsHeader.setText("Global Controls", juce::dontSendNotification);
        mLabelGlobalControlsHeader.setJustificationType(juce::Justification::centred);
        mLabelGlobalControlsHeader.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelGlobalControlsHeader.setFont(juce::Font(20.0f).boldened());
        addAndMakeVisible(mLabelGlobalControlsHeader);
        
        // Configure Stereo Controls header
        mLabelStereoControlsHeader.setText("Stereo Controls", juce::dontSendNotification);
        mLabelStereoControlsHeader.setJustificationType(juce::Justification::centred);
        mLabelStereoControlsHeader.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelStereoControlsHeader.setFont(juce::Font(20.0f).boldened());
        addAndMakeVisible(mLabelStereoControlsHeader);
        
        // Configure Filtering header
        mLabelFilteringHeader.setText("Filtering", juce::dontSendNotification);
        mLabelFilteringHeader.setJustificationType(juce::Justification::centred);
        mLabelFilteringHeader.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelFilteringHeader.setFont(juce::Font(20.0f).boldened());
        addAndMakeVisible(mLabelFilteringHeader);

    }
    
    void AudioPluginAudioProcessorEditor::makeSectionGroups() {
        // Configure Global Controls group
        mGroupGlobalControls.setText("");
        mGroupGlobalControls.setColour(juce::GroupComponent::outlineColourId, juce::Colours::black);
        mGroupGlobalControls.setColour(juce::GroupComponent::textColourId, juce::Colours::black);
        addAndMakeVisible(mGroupGlobalControls);
        
        // Configure Stereo Controls group
        mGroupStereoControls.setText("");
        mGroupStereoControls.setColour(juce::GroupComponent::outlineColourId, juce::Colours::black);
        mGroupStereoControls.setColour(juce::GroupComponent::textColourId, juce::Colours::black);
        addAndMakeVisible(mGroupStereoControls);
        
        // Configure Filtering group
        mGroupFiltering.setText("");
        mGroupFiltering.setColour(juce::GroupComponent::outlineColourId, juce::Colours::black);
        mGroupFiltering.setColour(juce::GroupComponent::textColourId, juce::Colours::black);
        addAndMakeVisible(mGroupFiltering);
    }

    // ============================================================================
    // UI COMPONENT SETUP METHODS - MAIN CONTROLS
    // ============================================================================
    
    void AudioPluginAudioProcessorEditor::makeNumVoicesControl() {
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
    
    void AudioPluginAudioProcessorEditor::makeVoiceAttenuationControl() {
        // Configure Voice Attenuation slider
        mSliderVoiceAttenuation.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderVoiceAttenuation.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderVoiceAttenuation.setRange(-6.0, 0.0, 0.1);
        mSliderVoiceAttenuation.setValue(0.0);
        mSliderVoiceAttenuation.setDoubleClickReturnValue(true, 0.0);
        
        // Set text box text colour to black
        mSliderVoiceAttenuation.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelVoiceAttenuation.setText("Voice Atten. (dB)", juce::dontSendNotification);
        mLabelVoiceAttenuation.setJustificationType(juce::Justification::centred);
        mLabelVoiceAttenuation.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelVoiceAttenuation.attachToComponent(&mSliderVoiceAttenuation, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderVoiceAttenuation);
        addAndMakeVisible(mLabelVoiceAttenuation);
        
        // Create parameter attachment for Voice Attenuation slider
        mVoiceAttenuationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "voiceAttenuation", mSliderVoiceAttenuation);
    }
    
    void AudioPluginAudioProcessorEditor::makeMasterVoiceLevelControl() {
        // Configure Master Voice Level slider
        mSliderMasterVoiceLevel.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderMasterVoiceLevel.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderMasterVoiceLevel.setRange(-20.0, 6.0, 0.1);
        mSliderMasterVoiceLevel.setValue(-3.0);
        mSliderMasterVoiceLevel.setDoubleClickReturnValue(true, -3.0);
        
        // Set text box text colour to black
        mSliderMasterVoiceLevel.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelMasterVoiceLevel.setText("Master Voice (dB)", juce::dontSendNotification);
        mLabelMasterVoiceLevel.setJustificationType(juce::Justification::centred);
        mLabelMasterVoiceLevel.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelMasterVoiceLevel.attachToComponent(&mSliderMasterVoiceLevel, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderMasterVoiceLevel);
        addAndMakeVisible(mLabelMasterVoiceLevel);
        
        // Create parameter attachment for Master Voice Level slider
        mMasterVoiceLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "masterVoiceLevel", mSliderMasterVoiceLevel);
    }
    
    void AudioPluginAudioProcessorEditor::makeRateControl() {
        // Configure Rate slider
        mSliderRate.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderRate.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderRate.setRange(0.1, 2.0, 0.01);
        mSliderRate.setValue(1.0);
        mSliderRate.setDoubleClickReturnValue(true, 1.0);
        
        // Set text box text colour to black
        mSliderRate.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelRate.setText("Rate (Hz)", juce::dontSendNotification);
        mLabelRate.setJustificationType(juce::Justification::centred);
        mLabelRate.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelRate.attachToComponent(&mSliderRate, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderRate);
        addAndMakeVisible(mLabelRate);
        
        // Create parameter attachment for Rate slider
        mRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "rate", mSliderRate);
    }
    
    void AudioPluginAudioProcessorEditor::makeDepthControl() {
        // Configure Depth slider
        mSliderDepth.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderDepth.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderDepth.setRange(0.0, 1.0, 0.01);
        mSliderDepth.setValue(0.5);
        mSliderDepth.setDoubleClickReturnValue(true, 0.5);
        
        // Set text box text colour to black
        mSliderDepth.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelDepth.setText("Depth", juce::dontSendNotification);
        mLabelDepth.setJustificationType(juce::Justification::centred);
        mLabelDepth.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelDepth.attachToComponent(&mSliderDepth, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderDepth);
        addAndMakeVisible(mLabelDepth);
        
        // Create parameter attachment for Depth slider
        mDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "depth", mSliderDepth);
    }
    
    void AudioPluginAudioProcessorEditor::makeMixControl() {
        // Configure Mix slider
        mSliderMix.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderMix.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderMix.setRange(0.0, 1.0, 0.01);
        mSliderMix.setValue(0.7);
        mSliderMix.setDoubleClickReturnValue(true, 0.7);
        
        // Set text box text colour to black
        mSliderMix.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelMix.setText("Mix", juce::dontSendNotification);
        mLabelMix.setJustificationType(juce::Justification::centred);
        mLabelMix.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelMix.attachToComponent(&mSliderMix, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderMix);
        addAndMakeVisible(mLabelMix);
        
        // Create parameter attachment for Mix slider
        mMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "mix", mSliderMix);
    }
    
    void AudioPluginAudioProcessorEditor::makeBaseDelayControl() {
        // Configure Base Delay slider
        mSliderBaseDelay.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderBaseDelay.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderBaseDelay.setRange(10.0, 100.0, 0.1);
        mSliderBaseDelay.setValue(30.0);
        mSliderBaseDelay.setDoubleClickReturnValue(true, 30.0);
        
        // Set text box text colour to black
        mSliderBaseDelay.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelBaseDelay.setText("Base Delay (ms)", juce::dontSendNotification);
        mLabelBaseDelay.setJustificationType(juce::Justification::centred);
        mLabelBaseDelay.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelBaseDelay.attachToComponent(&mSliderBaseDelay, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderBaseDelay);
        addAndMakeVisible(mLabelBaseDelay);
        
        // Create parameter attachment for Base Delay slider
        mBaseDelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "baseDelay", mSliderBaseDelay);
    }
    
    void AudioPluginAudioProcessorEditor::makeEnabledControl() {
        // Configure Enabled toggle button
        mToggleEnabled.setButtonText("Enabled");
        mToggleEnabled.setToggleState(true, juce::dontSendNotification);
        
        // Set text colour to black
        mToggleEnabled.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        
        // Set box border and tick mark to black
        mToggleEnabled.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::black);
        mToggleEnabled.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
        
        // Configure label
        mLabelEnabled.setText("", juce::dontSendNotification);
        mLabelEnabled.attachToComponent(&mToggleEnabled, true);
            
        // Add components to editor
        addAndMakeVisible(mToggleEnabled);
        addAndMakeVisible(mLabelEnabled);
        
        // Create parameter attachment for Enabled toggle
        mEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processorRef.getParameterTree(), "enabled", mToggleEnabled);
    }

    // ============================================================================
    // UI COMPONENT SETUP METHODS - STEREO SECTION
    // ============================================================================
    
    void AudioPluginAudioProcessorEditor::makeStereoModeControl() {
        // Configure Stereo Mode combo box
        mComboStereoMode.addItem("Mono", 1);
        mComboStereoMode.addItem("Stereo", 2);
        mComboStereoMode.addItem("Mid-Side", 3);
        mComboStereoMode.setSelectedId(1, juce::dontSendNotification);
        
        // Configure label
        mLabelStereoMode.setText("Stereo Mode", juce::dontSendNotification);
        mLabelStereoMode.setJustificationType(juce::Justification::centred);
        mLabelStereoMode.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelStereoMode.attachToComponent(&mComboStereoMode, true);
            
        // Add components to editor
        addAndMakeVisible(mComboStereoMode);
        addAndMakeVisible(mLabelStereoMode);
        
        // Create parameter attachment for Stereo Mode combo
        mStereoModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            processorRef.getParameterTree(), "stereoMode", mComboStereoMode);
    }
    
    void AudioPluginAudioProcessorEditor::makeStereoSpreadControl() {
        // Configure Stereo Spread slider
        mSliderStereoSpread.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderStereoSpread.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderStereoSpread.setRange(0.0, 1.0, 0.01);
        mSliderStereoSpread.setValue(0.5);
        mSliderStereoSpread.setDoubleClickReturnValue(true, 0.5);
        
        // Set text box text colour to black
        mSliderStereoSpread.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelStereoSpread.setText("Stereo Spread", juce::dontSendNotification);
        mLabelStereoSpread.setJustificationType(juce::Justification::centred);
        mLabelStereoSpread.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelStereoSpread.attachToComponent(&mSliderStereoSpread, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderStereoSpread);
        addAndMakeVisible(mLabelStereoSpread);
        
        // Create parameter attachment for Stereo Spread slider
        mStereoSpreadAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "stereoSpread", mSliderStereoSpread);
    }
    
    void AudioPluginAudioProcessorEditor::makeMidEnabledControl() {
        // Configure Mid Enabled toggle button
        mToggleMidEnabled.setButtonText("Mid Enabled");
        mToggleMidEnabled.setToggleState(true, juce::dontSendNotification);
        
        // Set text colour to black
        mToggleMidEnabled.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        
        // Set box border and tick mark to black
        mToggleMidEnabled.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::black);
        mToggleMidEnabled.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
        
        // Configure label
        mLabelMidEnabled.setText("", juce::dontSendNotification);
        mLabelMidEnabled.attachToComponent(&mToggleMidEnabled, true);
            
        // Add components to editor
        addAndMakeVisible(mToggleMidEnabled);
        addAndMakeVisible(mLabelMidEnabled);
        
        // Create parameter attachment for Mid Enabled toggle
        mMidEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processorRef.getParameterTree(), "midEnabled", mToggleMidEnabled);
    }
    
    void AudioPluginAudioProcessorEditor::makeSideEnabledControl() {
        // Configure Side Enabled toggle button
        mToggleSideEnabled.setButtonText("Side Enabled");
        mToggleSideEnabled.setToggleState(true, juce::dontSendNotification);
        
        // Set text colour to black
        mToggleSideEnabled.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        
        // Set box border and tick mark to black
        mToggleSideEnabled.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::black);
        mToggleSideEnabled.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
        
        // Configure label
        mLabelSideEnabled.setText("", juce::dontSendNotification);
        mLabelSideEnabled.attachToComponent(&mToggleSideEnabled, true);
            
        // Add components to editor
        addAndMakeVisible(mToggleSideEnabled);
        addAndMakeVisible(mLabelSideEnabled);
        
        // Create parameter attachment for Side Enabled toggle
        mSideEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processorRef.getParameterTree(), "sideEnabled", mToggleSideEnabled);
    }
    
    void AudioPluginAudioProcessorEditor::makeSideGainControl() {
        // Configure Side Gain slider
        mSliderSideGain.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderSideGain.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderSideGain.setRange(-20.0, 20.0, 0.1);
        mSliderSideGain.setValue(0.0);
        mSliderSideGain.setDoubleClickReturnValue(true, 0.0);
        
        // Set text box text colour to black
        mSliderSideGain.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelSideGain.setText("Side Gain (dB)", juce::dontSendNotification);
        mLabelSideGain.setJustificationType(juce::Justification::centred);
        mLabelSideGain.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelSideGain.attachToComponent(&mSliderSideGain, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderSideGain);
        addAndMakeVisible(mLabelSideGain);
        
        // Create parameter attachment for Side Gain slider
        mSideGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "sideGain", mSliderSideGain);
    }

    // ============================================================================
    // UI COMPONENT SETUP METHODS - FILTER SECTION
    // ============================================================================
    
    void AudioPluginAudioProcessorEditor::makeLPFEnabledControl() {
        // Configure LPF Enabled toggle button
        mToggleLPFEnabled.setButtonText("LPF Enabled");
        mToggleLPFEnabled.setToggleState(false, juce::dontSendNotification);
        
        // Set text colour to black
        mToggleLPFEnabled.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        
        // Set box border and tick mark to black
        mToggleLPFEnabled.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::black);
        mToggleLPFEnabled.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
        
        // Configure label
        mLabelLPFEnabled.setText("", juce::dontSendNotification);
        mLabelLPFEnabled.attachToComponent(&mToggleLPFEnabled, true);
            
        // Add components to editor
        addAndMakeVisible(mToggleLPFEnabled);
        addAndMakeVisible(mLabelLPFEnabled);
        
        // Create parameter attachment for LPF Enabled toggle
        mLPFEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processorRef.getParameterTree(), "lpfEnabled", mToggleLPFEnabled);
    }
    
    void AudioPluginAudioProcessorEditor::makeLPFCutoffControl() {
        // Configure LPF Cutoff slider
        mSliderLPFCutoff.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderLPFCutoff.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderLPFCutoff.setRange(20.0, 20000.0, 1.0);
        mSliderLPFCutoff.setValue(20000.0);
        mSliderLPFCutoff.setDoubleClickReturnValue(true, 20000.0);
        
        // Set text box text colour to black
        mSliderLPFCutoff.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelLPFCutoff.setText("LPF Cutoff (Hz)", juce::dontSendNotification);
        mLabelLPFCutoff.setJustificationType(juce::Justification::centred);
        mLabelLPFCutoff.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelLPFCutoff.attachToComponent(&mSliderLPFCutoff, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderLPFCutoff);
        addAndMakeVisible(mLabelLPFCutoff);
        
        // Create parameter attachment for LPF Cutoff slider
        mLPFCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "lpfCutoff", mSliderLPFCutoff);
    }
    
    void AudioPluginAudioProcessorEditor::makeHPFEnabledControl() {
        // Configure HPF Enabled toggle button
        mToggleHPFEnabled.setButtonText("HPF Enabled");
        mToggleHPFEnabled.setToggleState(false, juce::dontSendNotification);
        
        // Set text colour to black
        mToggleHPFEnabled.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        
        // Set box border and tick mark to black
        mToggleHPFEnabled.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::black);
        mToggleHPFEnabled.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
        
        // Configure label
        mLabelHPFEnabled.setText("", juce::dontSendNotification);
        mLabelHPFEnabled.attachToComponent(&mToggleHPFEnabled, true);
            
        // Add components to editor
        addAndMakeVisible(mToggleHPFEnabled);
        addAndMakeVisible(mLabelHPFEnabled);
        
        // Create parameter attachment for HPF Enabled toggle
        mHPFEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processorRef.getParameterTree(), "hpfEnabled", mToggleHPFEnabled);
    }
    
    void AudioPluginAudioProcessorEditor::makeHPFCutoffControl() {
        // Configure HPF Cutoff slider
        mSliderHPFCutoff.setSliderStyle(juce::Slider::LinearHorizontal);
        mSliderHPFCutoff.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
        mSliderHPFCutoff.setRange(20.0, 20000.0, 1.0);
        mSliderHPFCutoff.setValue(20.0);
        mSliderHPFCutoff.setDoubleClickReturnValue(true, 20.0);
        
        // Set text box text colour to black
        mSliderHPFCutoff.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            
        // Configure label
        mLabelHPFCutoff.setText("HPF Cutoff (Hz)", juce::dontSendNotification);
        mLabelHPFCutoff.setJustificationType(juce::Justification::centred);
        mLabelHPFCutoff.setColour(juce::Label::textColourId, juce::Colours::black);
        mLabelHPFCutoff.attachToComponent(&mSliderHPFCutoff, true);
            
        // Add components to editor
        addAndMakeVisible(mSliderHPFCutoff);
        addAndMakeVisible(mLabelHPFCutoff);
        
        // Create parameter attachment for HPF Cutoff slider
        mHPFCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getParameterTree(), "hpfCutoff", mSliderHPFCutoff);
    }

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
        
        const int componentSpacing = 40;  // Vertical spacing between control rows
        const int sectionSpacing = 60;    // Extra spacing between sections
        const int groupPaddingHorizontal = 50;  // Horizontal padding inside group boxes
        const int groupPaddingVertical = 15;    // Vertical padding inside group boxes
        int currentY = HEADER_HEIGHT + WINDOW_BORDER_MARGIN;
        
        // ============================================================================
        // MAIN CONTROLS SECTION
        // ============================================================================
        
        int globalControlsStartY = currentY;
        
        // Global Controls header
        mLabelGlobalControlsHeader.setBounds(WINDOW_BORDER_MARGIN + groupPaddingHorizontal, currentY + groupPaddingVertical, 
                                           getWidth() - (WINDOW_BORDER_MARGIN * 2) - (groupPaddingHorizontal * 2), COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Enabled (toggle button - positioned differently)
        int toggleX = WINDOW_BORDER_MARGIN + groupPaddingHorizontal;
        int toggleWidth = 100;
        mToggleEnabled.setBounds(toggleX, currentY, toggleWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Num. Voices
        int sliderX = WINDOW_BORDER_MARGIN + LABEL_WIDTH + groupPaddingHorizontal;
        int sliderWidth = getWidth() - (WINDOW_BORDER_MARGIN * 2) - LABEL_WIDTH - (groupPaddingHorizontal * 2);
        mSliderNumVoices.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Voice Attenuation
        mSliderVoiceAttenuation.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Master Voice Level
        mSliderMasterVoiceLevel.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Mix
        mSliderMix.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Rate
        mSliderRate.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Depth
        mSliderDepth.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Base Delay
        mSliderBaseDelay.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Position Global Controls group box
        mGroupGlobalControls.setBounds(WINDOW_BORDER_MARGIN, globalControlsStartY, 
                                     getWidth() - (WINDOW_BORDER_MARGIN * 2), 
                                     currentY - globalControlsStartY + groupPaddingVertical);
        currentY += sectionSpacing;
        
        // ============================================================================
        // STEREO SECTION
        // ============================================================================
        
        int stereoControlsStartY = currentY;
        
        // Stereo Controls header
        mLabelStereoControlsHeader.setBounds(WINDOW_BORDER_MARGIN + groupPaddingHorizontal, currentY + groupPaddingVertical, 
                                           getWidth() - (WINDOW_BORDER_MARGIN * 2) - (groupPaddingHorizontal * 2), COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Stereo Mode (combo box)
        mComboStereoMode.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Stereo Spread
        mSliderStereoSpread.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Mid Enabled
        mToggleMidEnabled.setBounds(toggleX, currentY, toggleWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Side Enabled
        mToggleSideEnabled.setBounds(toggleX, currentY, toggleWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Side Gain
        mSliderSideGain.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Position Stereo Controls group box
        mGroupStereoControls.setBounds(WINDOW_BORDER_MARGIN, stereoControlsStartY, 
                                     getWidth() - (WINDOW_BORDER_MARGIN * 2), 
                                     currentY - stereoControlsStartY + groupPaddingVertical);
        currentY += sectionSpacing;
        
        // ============================================================================
        // FILTER SECTION
        // ============================================================================
        
        int filteringStartY = currentY;
        
        // Filtering header
        mLabelFilteringHeader.setBounds(WINDOW_BORDER_MARGIN + groupPaddingHorizontal, currentY + groupPaddingVertical, 
                                      getWidth() - (WINDOW_BORDER_MARGIN * 2) - (groupPaddingHorizontal * 2), COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // LPF Enabled
        mToggleLPFEnabled.setBounds(toggleX, currentY, toggleWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // LPF Cutoff
        mSliderLPFCutoff.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // HPF Enabled
        mToggleHPFEnabled.setBounds(toggleX, currentY, toggleWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // HPF Cutoff
        mSliderHPFCutoff.setBounds(sliderX, currentY, sliderWidth, COMPONENT_HEIGHT);
        currentY += componentSpacing;
        
        // Position Filtering group box
        mGroupFiltering.setBounds(WINDOW_BORDER_MARGIN, filteringStartY, 
                                getWidth() - (WINDOW_BORDER_MARGIN * 2), 
                                currentY - filteringStartY + groupPaddingVertical);
    }
} // namespace audio_plugin