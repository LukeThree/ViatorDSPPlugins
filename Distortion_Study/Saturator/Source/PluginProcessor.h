/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define driveSliderId "drive"
#define driveSliderName "Drive"
#define trimSliderId "trim"
#define trimSliderName "Trim"
#define modelId "model"
#define modelName "Model"

//==============================================================================
/**
*/
class SaturatorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SaturatorAudioProcessor();
    ~SaturatorAudioProcessor() override;

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
    
    float scaleRange (const float &input, const float &inputLow, const float &inputHigh, const float &outputLow, const float &outputHigh);
    
    float softClip(const float &input, const float &drive);
    float hardClip(const float &input, const float &drive);
    float dcDistortion(const float &input, const float &drive);
    float diode(const float &input, const float &drive);
    float fullWaveRect(const float &input, const float &drive);
    
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    const float piDivisor = 2 / M_PI;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturatorAudioProcessor)
};
