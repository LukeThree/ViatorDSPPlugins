/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define inputGainSliderId "inputGain"
#define inputGainSliderName "Input Gain"

#define biasSliderId "bias"
#define biasSliderName "Bias"

#define outputGainSliderId "outputGain"
#define outputGainSliderName "Output Gain"

//==============================================================================
/**
*/
class Distortion_StudyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Distortion_StudyAudioProcessor();
    ~Distortion_StudyAudioProcessor() override;

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

    
private:
    
    using InputGainProcessor = juce::dsp::Gain<float>;
    using BiasProcessor = juce::dsp::Bias<float>;
    using WaveshapingProcessor = juce::dsp::WaveShaper<float>;
    using OutputGainProcessor = juce::dsp::Gain<float>;

    juce::dsp::ProcessorChain<InputGainProcessor, BiasProcessor, WaveshapingProcessor, OutputGainProcessor> distortionProcessor;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distortion_StudyAudioProcessor)
};
