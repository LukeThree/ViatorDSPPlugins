/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"

//==============================================================================
/**
*/
class Pastel_VerbAudioProcessor  :
public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    Pastel_VerbAudioProcessor();
    ~Pastel_VerbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    /**Create parameter trees*/
    juce::AudioProcessorValueTreeState treeState;
    juce::ValueTree variableTree;
    
    /**Helpful variables to store the current window size*/
    float windowWidth {0.0};
    float windowHeight {0.0};

private:
    
    /**Important parameter methods*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    /**DSP*/
    juce::dsp::LadderFilter<float> ladderProcessor;
    juce::dsp::Reverb reverbProcessor;
    juce::dsp::Reverb::Parameters reverbParams;
    
    /**Dialog box*/
    juce::AlertWindow alertWindow {"Alert Window", "Test", juce::AlertWindow::AlertIconType::WarningIcon};
    
    /**Recall saved parameters*/
    void setStateForTreeParameters();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pastel_VerbAudioProcessor)
};
