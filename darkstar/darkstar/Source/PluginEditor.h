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
class DarkstarAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DarkstarAudioProcessorEditor (DarkstarAudioProcessor&);
    ~DarkstarAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DarkstarAudioProcessor& audioProcessor;
    
    juce::Image background;
    
    viator_gui::PushButton pedalView, ampView, fxView;
    
    viator_gui::Dial dial1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DarkstarAudioProcessorEditor)
};
