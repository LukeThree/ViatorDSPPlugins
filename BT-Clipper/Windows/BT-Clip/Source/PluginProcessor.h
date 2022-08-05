/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LV_Clippers.h"
#include "LV_SVFilter.h"

#define filterToggleId "filter toggle"
#define filterToggleName "Post EQ"

#define phaseId "phase toggle"
#define phaseName "Phase Toggle"

#define driveModelId "drive model"
#define driveModelName "Drive Model"

#define inputSliderId "input slider"
#define inputSliderName "Input Slider"

#define outputSliderId "output slider"
#define outputSliderName "Output Slider"

#define cutoffSliderId "mid tone"
#define cutoffSliderName "Mid Tone"

#define midGainSliderId "mid gain"
#define midGainSliderName "Mid Gain"

#define driveSliderId "drive slider"
#define driveSliderName "Drive Slider"


//==============================================================================
/**
*/
class BTClipAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    BTClipAudioProcessor();
    ~BTClipAudioProcessor() override;

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
    
    /**Value Trees*/
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
    
    /**Window Vars*/
    float m_WindowWidth {0.0f};
    float m_WindowHeight {0.0f};

private:
    
    /**Parameters*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    void add_params();
    void remove_params();
    void set_state();
    
    /**DSP*/
    LV_Clippers m_ClippersModule;
    LV_SVFilter m_MidToneModule;
    
    float m_Preamp {0.0};
    float m_Trim {0.0};
    float m_Drive {0.0f};
    int m_Drive_Type {0};
    bool m_Phase {false};
    bool m_PostEq {false};
    
    /**DSP Modules*/
    void prepare_mid_tone(juce::dsp::ProcessSpec spec);
    void prepare_clipper(juce::dsp::ProcessSpec spec);
    void prepare_others();
    
    /**DSP Loop*/
    void do_dsp_loop(juce::AudioBuffer<float> &buffer);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BTClipAudioProcessor)
};
