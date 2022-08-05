/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define oneId "31"
#define oneName "31"

#define twoId "63"
#define twoName "63"

#define threeId "125"
#define threeName "125"

#define fourId "250"
#define fourName "250"

#define fiveId "500"
#define fiveName "500"

#define sixId "1K"
#define sixName "1K"

#define sevenId "2K"
#define sevenName "2K"

#define eightId "4K"
#define eightName "4K"

#define nineId "8K"
#define nineName "8K"

#define tenId "15K"
#define tenName "15K"

#define driveId "drive"
#define driveName "Drive"

#define trimSliderId "trim"
#define trimSliderName "Trim"

#define phaseId "phase"
#define phaseName "Phase"

#define lowBellId "low bell"
#define lowBellName "Low Bell"

#define highBellId "high bell"
#define highBellName "High Bell"

//==============================================================================
/**
*/
class GraphicEQAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    GraphicEQAudioProcessor();
    ~GraphicEQAudioProcessor() override;

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
    float lastSampleRate {44100.0f};
    
    float lastPreampGain, currentPreampGain, lastTrimGain, currentTrimGain;
    
    int currentPhase;
    
    float filter2Gain, filter9Gain, rawDrive;
    
    int lowBellState;
    int highBellState;
    
    // My functions
    void setPhase(int phase);
    void callProcessorCore(juce::AudioBuffer<float>& buffer, const int numChannels);
    
    void updateFilter1(float gain);
    void updateFilter2(float gain, int bell);
    void updateFilter3(float gain);
    void updateFilter4(float gain);
    void updateFilter5(float gain);
    void updateFilter6(float gain);
    void updateFilter7(float gain);
    void updateFilter8(float gain);
    void updateFilter9(float gain, int bell);
    void updateFilter10(float gain);

    
    float getShelfQ(float value);
    float getPeakQ(float value);
    float getGain(float gain);
    
    // Parameter function
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
        
    // Parameter listener function
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    // dsp modules
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter1;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter2;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter3;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter4;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter5;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter6;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter7;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter8;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter9;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter10;
    //juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> disLP;
    //juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> disHS1;
    //juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> disHS2;
    //juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> disHS3;
    
    std::vector<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>*> filters {&filter1, &filter2, &filter3, &filter4, &filter5, &filter6, &filter7, &filter8, &filter9, &filter10};

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicEQAudioProcessor)
};
