/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "LV_Compressor.h"
#include "LV_SVFilter.h"

//==============================================================================
/**
*/
class BedroomCompAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    BedroomCompAudioProcessor();
    ~BedroomCompAudioProcessor() override;

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
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    float getRatioForDial();
    std::string getSpeedDialStrings();
    
    float getSignal();
    float getSampleRate();
    float getBufferSize();
    float currentSignal;
    float currentSampleRate {44100.0};
    float currentBufferSize {64.0};
    float loopCount {0};
    float seconds {0};
    
    LV_Compressor compressorModule;
    LV_SVFilter compressorFilterModule;
    LV_SVFilter inputHPFilterModule;


private:
    
    bool firstRun = false;

    std::vector<LV_SVFilter*> hpFilters
    {
        &compressorFilterModule, &inputHPFilterModule
    };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BedroomCompAudioProcessor)
};
