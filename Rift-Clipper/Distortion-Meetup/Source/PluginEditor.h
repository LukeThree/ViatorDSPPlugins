/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI-Components/UI_Controls.h"

//==============================================================================
/**
*/
class DistortionMeetupAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistortionMeetupAudioProcessorEditor (DistortionMeetupAudioProcessor&);
    ~DistortionMeetupAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    
    DistortionMeetupAudioProcessor& audioProcessor;
    
    UI_Controls controls;
    
    juce::Slider bandOneSlider;
    juce::Slider bandTwoSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionMeetupAudioProcessorEditor)
};
