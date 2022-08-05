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
class ReverbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReverbAudioProcessorEditor (ReverbAudioProcessor&);
    ~ReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider cutoffSlider, resonanceSlider, driveSlider, roomSizeSlider, dampingSlider, widthSlider, drySlider, wetSlider;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttach, resonanceSliderAttach, driveSliderAttach,
    roomSizeSliderAttach, dampingSliderAttach, widthSliderAttach, drySliderAttach, wetSliderAttach;
        
    juce::TextButton filterEngageButton, filterModeButton;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> filterEngageButtonAttach, filterModeButtonAttach;
    
    juce::Label cutoffLabel, resonanceLabel, driveLabel, roomLabel, dampingLabel, widthLabel, dryLabel, wetLabel;
        
    ViatorDial customDial;
    
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    ReverbAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
