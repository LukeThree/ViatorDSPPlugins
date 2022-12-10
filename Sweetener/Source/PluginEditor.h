/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI-Components/MultiBandWindow.h"

//==============================================================================
/**
 */ 

class SweetenerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SweetenerAudioProcessorEditor (SweetenerAudioProcessor&);
    ~SweetenerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SweetenerAudioProcessor& audioProcessor;
    
    void initWindow();
    MultiBandWindow _multiBandWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SweetenerAudioProcessorEditor)
};
