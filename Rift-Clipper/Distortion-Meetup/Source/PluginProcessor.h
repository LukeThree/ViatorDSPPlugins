/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./DSP/LV_SplitDistortion.h"

#define inputID "input"
#define inputName "Input"

#define driveID "curve"
#define driveName "Curve"

#define crossOverID "cross over"
#define crossOverName "Cross Over"

#define bandChoiceID "band choice"
#define bandChoiceName "Band Choice"

#define modelChoiceID "model choice"
#define modelChoiceName "Model Choice"

#define mixID "mix"
#define mixName "Mix"

#define outputID "output"
#define outputName "Output"

#define clipInputID "clip input"
#define clipInputName "Clip Input"

//==============================================================================
/**
*/
class DistortionMeetupAudioProcessor  :
public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DistortionMeetupAudioProcessor();
    ~DistortionMeetupAudioProcessor() override;

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
    
    /** Value Trees =====================================================*/
    juce::AudioProcessorValueTreeState treeState;
    
    float bandOneCutoffMult = 1.0;
    float bandTwoCutoffMult = 1.0;

private:
    
    /** Parameters ======================================================*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    /** DSP ======================================================*/
    LV_SplitDistortion splitDistortionModule;
    void prepareSplitDistortion(juce::dsp::ProcessSpec spec);
    //==============================================================================
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::FIR::Coefficients<float>> bandOneFilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionMeetupAudioProcessor)
};
