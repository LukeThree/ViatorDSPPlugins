/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LV_TubeModel.h"
#include "Parameters.h"

//==============================================================================
/**
*/
class ElectronEQAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ElectronEQAudioProcessor();
    ~ElectronEQAudioProcessor() override;

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

    juce::AudioProcessorValueTreeState treeState;
    juce::ValueTree variableTree;

    float windowWidth {0.0}, windowHeight {0.0};

private:
    
    void prepareFilters(juce::dsp::ProcessSpec spec);
    void prepareTube(juce::dsp::ProcessSpec spec);
    void prepareTrim(juce::dsp::ProcessSpec spec);
    
    void recallWindow();
    void recallFilters();
    void recallTube();
    void recallTrim();
    void recallStereoChoice();
    void recallEqBypass();
    
    // Parameter functions
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    float output, lastSampleRate, projectSampleRate;
    
    bool osModuleToggle = false;
    
    int stereoMenuChoice {0};
    
    LV_TubeModel tubeModule;
    LV_SVFilter lowBoostModule, lowCutModule, highBoostModule, highCutModlue, lpfModule;
    
    juce::dsp::Oversampling<float> osModule;
    juce::dsp::Gain<float> trimModule;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ElectronEQAudioProcessor)
};
