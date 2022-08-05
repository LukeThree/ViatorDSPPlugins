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
class OverdrivePedalAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    OverdrivePedalAudioProcessorEditor (OverdrivePedalAudioProcessor&);
    ~OverdrivePedalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;

private:

    juce::Slider inputSlider, toneSlider, driveSlider, trimSlider;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> inputAttach, driveAttach, toneAttach, trimAttach;
    
    juce::Label inputSliderLabel, toneSliderLabel, driveSliderLabel, trimSliderLabel;
    
    ViatorDial customDial;
        
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    OverdrivePedalAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdrivePedalAudioProcessorEditor)
};
