/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./Parameters/ParamDefs.h"
#include "./DSP/LV_SVFilter.h"
#include "./DSP/LV_Clipper.h"

//==============================================================================
/**
*/
class BTClipperAudioProcessor  :
public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    BTClipperAudioProcessor();
    ~BTClipperAudioProcessor() override;

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
    
    juce::AudioVisualiserComponent scopeModule {1};
    
private:
    
    /** Parameters ======================================================*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    juce::dsp::Oversampling<float> oversamplingModule;
    float overSampleRate {44100.0f};
    float projectSampleRate {44100.0f};
    bool oversamplingState = false;
    
    LV_SVFilter midToneFilter;
    
    LV_Clipper clipperModule;
    float clipTypeChoice {1.0f};
    void initClipper();
    
    LV_SVFilter preToneModule;
    void initPreTone();
    
    LV_SVFilter postToneModule;
    void initPostTone();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BTClipperAudioProcessor)
};
