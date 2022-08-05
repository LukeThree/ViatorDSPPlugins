/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SliderGroup.h"

//==============================================================================
/**
*/

class NewComponent : public juce::Component
{
public:
    NewComponent()
    {
        
    }
    
    ~NewComponent() override
    {
        
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black.withAlpha(0.5f));
    }
    
    void resized() override
    {
        
    }
    
    void mouseDoubleClick (const juce::MouseEvent &event) override
    {
        DBG("Double Clicked!");
    }
    
private:
};

class MidiMapTestAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MidiMapTestAudioProcessorEditor (MidiMapTestAudioProcessor&);
    ~MidiMapTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiMapTestAudioProcessor& audioProcessor;
    
    SliderGroup sliderGroup;
    
    viator_gui::PushButton learnButton;
    viator_gui::PushButton macroLearnButton;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> learnAttach;
    
    NewComponent newComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMapTestAudioProcessorEditor)
};
