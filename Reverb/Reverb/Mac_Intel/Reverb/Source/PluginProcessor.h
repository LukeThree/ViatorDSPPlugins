/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define filterEngageId "filter engage"
#define filterEngageName "Filter Engage"

#define filterModeId "filter mode"
#define filterModeName "Filter Mode"

#define cutoffSliderId "cutoff"
#define cutoffSliderName "Cutoff"

#define resonanceSliderId "resonance"
#define resonanceSliderName "Resonance"

#define driveSliderId "drive"
#define driveSliderName "Drive"

#define roomSizeSliderId "room size"
#define roomSizeSliderName "Room Size"

#define dampingSliderId "damping"
#define dampingSliderName "Damping"

#define widthSliderId "width"
#define widthSliderName "Width"

#define drySliderId "dry"
#define drySliderName "Dry"

#define wetSliderId "wet"
#define wetSliderName "Wet"

//==============================================================================
/**
*/
class ReverbAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ReverbAudioProcessor();
    ~ReverbAudioProcessor() override;

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
    
    //bool filterEngageState;

private:
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    juce::dsp::LadderFilter<float> ladderProcessor;
    juce::dsp::Reverb reverbProcessor;
    juce::dsp::Reverb::Parameters reverbParams;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessor)
};
