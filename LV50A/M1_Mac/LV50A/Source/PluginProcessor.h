/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LV_SVFIlter.h"

#define driveSliderId "drive"
#define driveSliderName "Drive"

#define trimSliderId "trim"
#define trimSliderName "Trim"

#define phaseId "phase"
#define phaseName "Phase"

#define lowShelfBId "low shelf"
#define lowShelfBName "Low Shelf"

#define highShelfBId "high shelf"
#define highShelfBName "High Shelf"

#define filtersBId "filters"
#define filtersBName "Filters"

#define menuId "menu"
#define menuName "Menu"

// Filter Dials
#define gain1Id "band 1 gain"
#define gain1Name "Band 1 Gain"
#define gain2Id "band 2 gain"
#define gain2Name "Band 2 Gain"
#define gain3Id "band 3 gain"
#define gain3Name "Band 3 Gain"
#define gain4Id "band 4 gain"
#define gain4Name "Band 4 Gain"

#define freq1Id "band 1 freq"
#define freq1Name "Band 1 Freq"
#define freq2Id "band 2 freq"
#define freq2Name "Band 2 Freq"
#define freq3Id "band 3 freq"
#define freq3Name "Band 3 Freq"
#define freq4Id "band 4 freq"
#define freq4Name "Band 4 Freq"

//==============================================================================
/**
*/
class LV50AAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    LV50AAudioProcessor();
    ~LV50AAudioProcessor() override;

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
    
    // Value tree
    juce::AudioProcessorValueTreeState treeState;
    juce::ValueTree variableTree
    { "Variables", {},
      {
        { "Group", {{ "name", "Vars" }},
          {
              { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                { "Parameter", {{ "id", "height" }, { "value", 0.0 }}},
          }
        }
      }
    };
    
    /** Window Vars =====================================================*/
    float windowWidth {0.0f};
    float windowHeight {0.0f};
    
private:
    
    // Variables
    float lastSampleRate, projectSampleRate;
    
    float lastTrimGain, currentTrimGain, mRawDrive;
    
    int currentPhase;
    
    bool oversamplingToggle;
    
    // My functions
    void setPhase(int phase);
    
    void callProcessorCore(juce::dsp::AudioBlock<float> block, const int numChannels);
    
    void prepareEQFilters(juce::dsp::ProcessSpec spec);
    
    void prepareFilters(juce::dsp::ProcessSpec spec);
    
    float proportionalShelfFilterQ(float gain, bool lowShelf);
    
    float proportionalPeakFilterQ(float gain);
    
    void setAllSampleRates(float value);
    
    // Parameter function
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // Parameter listener function
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    // dsp modules
    LV_SVFilter mLSFilter, mBPFilter1, mBPFilter2, mHSFilter, mLPFilter, mHPFilter;
    
    juce::dsp::Oversampling<float> oversamplingProcessor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV50AAudioProcessor)
};
