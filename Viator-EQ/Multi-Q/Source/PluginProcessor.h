/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./DSP/GraphicEQ.h"
#include "./DSP/ParametricEQ.h"
#include "./DSP/TubeEQ.h"
#include "./Parameters/Parameters.h"

//==============================================================================
/**
*/
class MultiQAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    MultiQAudioProcessor();
    ~MultiQAudioProcessor() override;

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

    void updateGraphicParameters()
    {
        graphicEQModule.setStereoType(static_cast<int>(treeState.getRawParameterValue(msID)->load()));
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter1Gain, treeState.getRawParameterValue(graphicFilter1GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter2Gain, treeState.getRawParameterValue(graphicFilter2GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter3Gain, treeState.getRawParameterValue(graphicFilter3GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter4Gain, treeState.getRawParameterValue(graphicFilter4GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter5Gain, treeState.getRawParameterValue(graphicFilter5GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter6Gain, treeState.getRawParameterValue(graphicFilter6GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter7Gain, treeState.getRawParameterValue(graphicFilter7GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter8Gain, treeState.getRawParameterValue(graphicFilter8GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter9Gain, treeState.getRawParameterValue(graphicFilter9GainID)->load());
        graphicEQModule.setParameter(GraphicEQ<float>::ParameterId::kFilter10Gain, treeState.getRawParameterValue(graphicFilter10GainID)->load());
    }

    void updateParametricParameters()
    {
        parametricEQModule.setStereoType(static_cast<int>(treeState.getRawParameterValue(msID)->load()));
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter1Gain, treeState.getRawParameterValue(parametricFilter1GainID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter2Gain, treeState.getRawParameterValue(parametricFilter2GainID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter3Gain, treeState.getRawParameterValue(parametricFilter3GainID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter4Gain, treeState.getRawParameterValue(parametricFilter4GainID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter1Freq, treeState.getRawParameterValue(parametricFilter1FreqID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter2Freq, treeState.getRawParameterValue(parametricFilter2FreqID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter3Freq, treeState.getRawParameterValue(parametricFilter3FreqID)->load());
        parametricEQModule.setParameter(ParametricEQ<float>::ParameterId::kFilter4Freq, treeState.getRawParameterValue(parametricFilter4FreqID)->load());
    }

    void updateTubeEQParameters()
    {
        tubeEQModule.setStereoType(static_cast<int>(treeState.getRawParameterValue(msID)->load()));
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kLowBoostGain, treeState.getRawParameterValue(tubeLowBoostID)->load());
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kLowCutGain, treeState.getRawParameterValue(tubeLowCutID)->load() * -1.0);
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kHighBoostGain, treeState.getRawParameterValue(tubeHighBoostID)->load());
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kHighCutGain, treeState.getRawParameterValue(tubeHighCutID)->load() * -1.0);
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kLowFilterFreq, treeState.getRawParameterValue(tubeLowFreqID)->load());
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kHighFilterFreq, treeState.getRawParameterValue(tubeHighFreqID)->load());
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kBandWidth, treeState.getRawParameterValue(tubeFilterBWID)->load());
        tubeEQModule.setParameter(TubeEQ<float>::ParameterId::kDrive, treeState.getRawParameterValue(driveID)->load());
    }
    
    float getCPULoad();

private:
    
    juce::AudioProcessLoadMeasurer cpuMeasureModule;
    std::atomic<float> cpuLoad;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    /** DSP */
    juce::dsp::Oversampling<float> oversamplingModule;
    juce::dsp::ProcessSpec spec;
    void updateCommonParameters();
    bool osToggle = false;
    bool phaseToggle = false;
    
    GraphicEQ<float> graphicEQModule;
    
    ParametricEQ<float> parametricEQModule;
    
    TubeEQ<float> tubeEQModule;
    
    viator_dsp::SVFilter<float> hpFilter;
    viator_dsp::SVFilter<float> lpFilter;
    juce::dsp::Gain<float> gainModule;
    
    int bufferSize;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiQAudioProcessor)
};
