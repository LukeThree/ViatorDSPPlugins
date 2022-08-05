/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class GainStagerAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    GainStagerAudioProcessor();
    ~GainStagerAudioProcessor() override;

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
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::ValueTree variableTree
    { "Variables", {},
      {
        { "Group", {{ "name", "RMS Vars" }},
          {
            { "Parameter", {{ "id", "temp peak" }, { "value", 0.0 }}},
            { "Parameter", {{ "id", "temp average" },     { "value", 0.0 }}},
              { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                { "Parameter", {{ "id", "height" }, { "value", 0.0 }}}
          }
        }
      }
    };
    
    float windowWidth {0.0}, windowHeight {0.0};
    
private:
    
    // Variables
    float currentSampleRate, currentPhase;
    float lastPreampGain, currentPreampGain;
    
    float thresh {-18.0f};

    float n {5.0f};
    float avgRMSRead, avgRMSWrite, avgRMS{ 0.0f }, dip;
    int loopCount{ 0 }, loopLimit;

    bool averageHasRun = false;
    bool listen = false;
    
    int stageType {0};
    int calibration {0};
    
    float tempPeak {0.0f};
    float tempAvg {0.0f};
    float tempMedian {0.0f};
    
    // My functions
    void setPhase(int phase);
    float getGain(float value);
    float roundToTwo(const float input);
    
    float getPeakSignal(std::vector<float> vec, float rms);
    float getAverageSignal(std::vector<float> vec, float rms);
    float getModeSignal(std::vector<float> vec, float rms);
    
    std::vector<float> avgVector;
    
    // Parameter listener function
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    // dsp modules
    juce::dsp::Gain<float> preampProcessor;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainStagerAudioProcessor)
};
