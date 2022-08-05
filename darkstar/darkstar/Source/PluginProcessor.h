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
class DarkstarAudioProcessor  : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DarkstarAudioProcessor();
    ~DarkstarAudioProcessor() override;

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
    
    /** Pedal DSP ==============================================================*/
    viator_dsp::SVFilter pedalLPFilter;
    viator_dsp::SVFilter pedalHPFilter;
    viator_dsp::WaveShaper pedalShaper;
    juce::dsp::Gain<float> pedalLevelModule;
    void processOverdrivePedal(juce::dsp::AudioBlock<float>& inputBlock);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DarkstarAudioProcessor)
};
