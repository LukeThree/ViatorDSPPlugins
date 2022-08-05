/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./DSP/LV_Rectifier.h"

#define bandwidthID "speaker range"
#define bandwidthName "Speaker Range"

#define centerID "center freq"
#define centerName "Center Freq"

#define resoID "resonance"
#define resoName "Resonance"

#define driveID "dirt"
#define driveName "Dirt"

#define lowShelfID "low shelf gain"
#define lowShelfName "Low Shelf Gain"

#define highShelfID "high shelf gain"
#define highShelfName "High Shelf Gain"

//==============================================================================
/**
*/
class PhonemodelAudioProcessor  :
public juce::AudioProcessor
,public juce::AudioProcessorValueTreeState::Listener

{
public:
    //==============================================================================
    PhonemodelAudioProcessor();
    ~PhonemodelAudioProcessor() override;

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
    
    /** Parameters ======================================================*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    /** DSP ==============================================================*/
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midToneFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowCompensateFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowShelfFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highShelfFilter;
    
    juce::dsp::Gain<float> filterGainCompensateModule;
    
    LV_Rectifier inputRectifierModule;
    LV_Rectifier outputRectifierModule;
    
    void updateLowFilter();
    void updateHighFilter();
    void updateMidToneFilter(float freq, float q, float gain);
    void updateLowCompensateFilter();
    void updateLowShelfFilter();
    void updateHighShelfFilter();
    
    float calcLowCutoff(float centerFreq);
    float calcHighCutoff(float centerFreq);
    void initCutoffMultipliers(float newValue);
    void initCutoffs(float newValue);
    
    void updateAllFilters();
    
    int initDN() const;
    
    float lowCutoff {20.0f};
    float highCutoff {1000.0f};
    float lowCutoffMultiplier {1.0f};
    float highCutoffMultiplier {1.0f};
    float centerFreq {800.0f};
    float qWidth {0.33f};
    float midGain {0.0f};
    float drive {0.0f};
    float lowShelfGain {1.0f};
    float highShelfGain {1.0f};
    float rampInSeconds {0.01f};
    
    float lastSampleRate;
    
    void prepareDSP(juce::dsp::ProcessSpec& spec);
    void initDSPVariables();
    void initFilters();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhonemodelAudioProcessor)
};
