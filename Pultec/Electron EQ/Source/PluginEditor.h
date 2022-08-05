/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ViatorDial.h"
#include "StyleSheet.h"


//==============================================================================
/**
*/
class ElectronEQAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    ElectronEQAudioProcessorEditor (ElectronEQAudioProcessor&);
    ~ElectronEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider *slider) override;

private:
    
    // Background
    juce::Image pluginBackground;
    
    // Logo
    juce::Image headerLogo;
    
    // Menu box
    juce::ComboBox stereoMenu;
    void setStereoBoxProps(juce::ComboBox &menu);
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> stereoMenuAttach;

    // Shadow
    void setShadowProps();
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    juce::DropShadow sliderShadowProperties;
    juce::DropShadowEffect sliderShadow;
    
    // Custom LAF
    ViatorDial customDial;
    ViatorDial2 customDial2;
    
    // Buttons ===================================================================
    juce::TextButton enabledToggle, oversampleToggle;
    void initButtons();
    void setCommonButtonProps(juce::TextButton &button);
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> oversampleToggleAttach, enabledToggleAttach;
    
    std::vector<juce::TextButton*> buttons =
    {
        &enabledToggle, &oversampleToggle
    };
    
    // Sliders ===================================================================
    juce::Slider lowBoostDial, lowCutDial, lowFreqDial, bandwidthDial,
    highBoostDial, highCutDial, highFreqDial, lowPassSlider, driveSlider, trimSlider;
    
    juce::LV_FaderLAF faderSlider;

    void initDials();
    void setCommonDialProps(juce::Slider &dial);
    void setCommonSliderProps(juce::Slider &slider);
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment>
    preampDialAttach, driveSliderAttach, trimDialAttach, lowPassSliderAttach, highBoostDialAttach,
    highFreqDialAttach, highCutDialAttach, lowBoostDialAttach,
    lowFreqDialAttach, lowCutDialAttach, bandwidthDialAttach;

    
    std::vector<juce::Slider*> dials =
    {
        &lowBoostDial, &lowCutDial, &lowFreqDial, &bandwidthDial,
        &highBoostDial, &highCutDial, &highFreqDial
    };
    
    std::vector<juce::Slider*> sliders =
    {
        &lowPassSlider, &driveSlider, &trimSlider
    };
    
    // Labels ===================================================================
    juce::Label lowBostDialLabel, lowCutDialLabel, lowFreqDialLabel, bandwidthDialLabel,
    highBoostDialLabel, highCutDialLabel, highFreqDialLabel, lowPassSliderLabel, driveSliderLabel, trimSliderLabel;
    void initLabels();
    void setCommonLabelProps(juce::Label &label);
    
    std::vector<juce::Label*> dialLabels =
    {
        &lowBostDialLabel, &lowCutDialLabel, &lowFreqDialLabel, &bandwidthDialLabel,
        &highBoostDialLabel, &highCutDialLabel, &highFreqDialLabel
    };
    
    std::vector<std::string> dialLabelTexts
    {
        "Low Boost", "Low Atten", "Freq ", "Bandwidth", "High Boost", "High Atten", "Freq"
    };
    
    std::vector<juce::Label*> sliderLabels =
    {
        &lowPassSliderLabel, &driveSliderLabel, &trimSliderLabel
    };
    
    std::vector<std::string> sliderLabelTexts
    {
        "LP", "Drive", "Trim"
    };

    ElectronEQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ElectronEQAudioProcessorEditor)
};
