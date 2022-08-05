/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define inputSliderId "input"
#define inputSliderName "Input"

#define driveSliderId "drive"
#define driveSliderName "Drive"

#define toneSliderId "tone"
#define toneSliderName "Tone"

#define trimSliderId "trim"
#define trimSliderName "Trim"

//==============================================================================
/**
*/
class OverdrivePedalAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    OverdrivePedalAudioProcessor();
    ~OverdrivePedalAudioProcessor() override;

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
    
    //==============================================================================
    void updateDrive(const float &drive);
    
    float convertToDrive(const float drive);
    
    float getTanhOf(const float input);
    
    float getRandomNumber(float timeElapsed);
    
    float calculateDeJongMap(float x);
    
    void prepareFilters(juce::dsp::ProcessSpec spec);
    
    void prepareGains(juce::dsp::ProcessSpec spec);
    
    void prepareDrive();
    
    void updateHSFilter(const float &gain);
    
    void callPreOSChain(juce::dsp::AudioBlock<float> block);
    
    void callPostOSChain(juce::dsp::AudioBlock<float> block);
    
    void callProcessorCore(juce::dsp::AudioBlock<float> block, const int numChannels);
    //==============================================================================
    
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    float lastSampleRate {44100.0f};
    float mDrive {-4.0f};
    float randomNumber = 0.69f;

private:
    
    float lastPreampGain, currentPreampGain, lastTrimGain, currentTrimGain;
    float lpFilterCutoff = 4000.0f;
    
    juce::Random random, whiteNoise;
    
    juce::dsp::LadderFilter<float> lpFilter;
    juce::dsp::StateVariableTPTFilter<float> hpFilter;
    
    juce::dsp::Oversampling<float> oversamplingProcessor;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> hsFilter;
    
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdrivePedalAudioProcessor)
};
