/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ViatorDial.h"

//==============================================================================
/**
*/
class DiodeAmplifierAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DiodeAmplifierAudioProcessorEditor (DiodeAmplifierAudioProcessor&);
    ~DiodeAmplifierAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    // Sliders
    juce::Slider inputSlider, driveSlider, lowSlider, midSlider, highSlider, outputSlider;
    std::vector<juce::Slider*> sliders;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> inputSliderAttach, driveSliderAttach, lowSliderAttach, midSliderAttach, highSliderAttach, outputSliderAttach;
        
    // Buttons
    void setCabButtonProps();
    juce::TextButton brightButton, cabButton, cabToggleButton, resetIRButton;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> brightButtonAttach, cabToggleAttach;

    // Window
    juce::GroupComponent windowBorder;
    
    // Logo
    juce::Image mLogo;
        
    // Background
    juce::Image pluginBackground;
        
    // Labels
    juce::Label inputLabel, driveLabel, lowLabel, midLabel, highLabel, outputLabel;
    std::vector<juce::Label*> labels;
    std::string inputSliderLabelText = "Input";
    std::string driveSliderLabelText = "Drive";
    std::string lowSliderLabelText = "Low";
    std::string midSliderLabelText = "Mid";
    std::string highSliderLabelText = "High";
    std::string outputSliderLabelText = "Output";
    std::vector<std::string> labelTexts;
        
    // Custom LAF
    ViatorDial customDial;
    ViatorDial2 customDial2;

    // Shadow
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    // Menu box
    juce::ComboBox mSampleMenu;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> mSampleMenuAttach;
    
    juce::AlertWindow settingsDialog {"Settings Window",
            "Congrats, you opened the window, but it doesn't do anything", juce::AlertWindow::AlertIconType::InfoIcon};
    
    DiodeAmplifierAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodeAmplifierAudioProcessorEditor)
};
