/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FirstPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FirstPluginAudioProcessorEditor (FirstPluginAudioProcessor&);
    ~FirstPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider gainSlider;
    juce::TextButton phaseInvertButton {"Phase"};
    juce::Label gainSliderLabel;
    
    FirstPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstPluginAudioProcessorEditor)
};
