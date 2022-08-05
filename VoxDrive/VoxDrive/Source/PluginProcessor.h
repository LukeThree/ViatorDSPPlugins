/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./Parameters/Parameters.h"
#include "./DSP/VoxDistortion.h"

//==============================================================================
/**
*/
class VoxDriveAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    VoxDriveAudioProcessor();
    ~VoxDriveAudioProcessor() override;

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
    
    
    juce::ValueTree variableTree {"Variables"};
    
    /** Window Vars =====================================================*/
    float windowWidth {0.0f};
    float windowHeight {0.0f};
    
    void updateParameters();
    
    juce::AudioProcessorValueTreeState treeState;
        
private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    static juce::String valueToTextFunction(float x) { return juce::String(x, 0); }
    static float textToValueFunction(const juce::String& str) { return static_cast<int>(str.getFloatValue()); }
    
    /** DSP */
    VoxDistortion<float> voxDistortionModule;
    juce::dsp::Oversampling<float> oversamplingModule;
    juce::dsp::ProcessSpec spec;
    viator_dsp::SVFilter<float> lowShelf;
    juce::dsp::Gain<float> outModule;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoxDriveAudioProcessor)
};
