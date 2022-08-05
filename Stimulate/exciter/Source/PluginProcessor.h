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
class ExciterAudioProcessor  : public juce::AudioProcessor
,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ExciterAudioProcessor();
    ~ExciterAudioProcessor() override;

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
    
    /** Spectrum analyzer */
    enum
    {
        fftOrder  = 11,
        fftSize   = 1 << fftOrder,
        scopeSize = 1024
    };
        
    /** FFT public vars */
    bool nextFFTBlockReady = false;
    void drawNextFrameOfSpectrum();
    float scopeData [scopeSize];
    void pushNextSampleIntoFifo (float sample) noexcept;

private:
    
    /** Parameters ======================================================*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    // Variables
    juce::SmoothedValue<float> rawGain {1.0};
    juce::SmoothedValue<float> mix {0.0};
    juce::SmoothedValue<float> oddMix {0.5};
    juce::SmoothedValue<float> evenMix {0.5};
    juce::SmoothedValue<float> cutoff {8000.0};
    juce::SmoothedValue<float> rawTrim {1.0};
    
    bool osToggle {false};
    bool totalPhase {false};
    
    int getPhase(bool currentPhaseParam);
    
    /** DSP */
    juce::dsp::LinkwitzRileyFilter<float> topBandFilter;
    juce::dsp::LinkwitzRileyFilter<float> bottomBandFilter;
    juce::dsp::LinkwitzRileyFilter<float> highPassFilter;
    juce::dsp::Oversampling<float> oversamplingModel;
    
    void stimulationBlock(juce::dsp::AudioBlock<float> &currentBlock);
    void lfoBlock(juce::dsp::AudioBlock<float> &currentBlock);
    void updateParameters();
    
    juce::dsp::ProcessSpec spec;
    
    /** Spectrum analyzer */
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
         
    /** FFT private vars */
    float fifo [fftSize];
    float fftData [2 * fftSize];
    int fifoIndex = 0;
    
    juce::dsp::Oscillator<float> oscModule { [] (float x)
    {
        return std::sin (x);
    }};
    
    juce::dsp::AudioBlock<float> oscBlock;
    viator_dsp::LFOGenerator lfoGenerator;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExciterAudioProcessor)
};
