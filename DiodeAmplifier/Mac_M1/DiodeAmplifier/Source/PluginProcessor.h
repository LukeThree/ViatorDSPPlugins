/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define inputGainSliderId "input"
#define inputGainSliderName "Input"

#define driveSliderId "drive"
#define driveSliderName "Drive"

#define lowSliderId "low"
#define lowSliderName "Low"

#define midSliderId "mid"
#define midSliderName "Mid"

#define highSliderId "high"
#define highSliderName "High"

#define outputGainSliderId "output"
#define outputGainSliderName "Output"

#define brightId "bright"
#define brightName "Bright"

#define cabId "cab"
#define cabName "Cab"

#define menuId "menu"
#define menuName "Menu"

//==============================================================================
/**
*/
class DiodeAmplifierAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DiodeAmplifierAudioProcessor();
    ~DiodeAmplifierAudioProcessor() override;

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
    void updateHighPassFilter(const float &freq);
    void updatePreClipFilter(const float &freq);
    void updateLowFilter(const float &gain);
    void updateMidFilter(const float &gain);
    void updateHighFilter(const float &gain);

    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::ValueTree variableTree;
    
    juce::File savedFile, root;
    std::unique_ptr<juce::File> location;

    juce::dsp::Convolution convolutionProcessor{juce::dsp::Convolution::Latency{0}};

    
private:
    const float piDivisor = 2.0 / 3.14;
    double lastSampleRate;
    double projectSampleRate {44100.0};
    float driveScaled;
    bool convolutionToggle, oversamplingToggle;
    
    void setAllSampleRates(float value);
    
    /* non user controlled filters. Used to shape the tone of the sim*/
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> preClipFilter;

    /*user controlled filters for the amp head*/
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highFilter;
    
    // Fuck 4k filter
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highNotchFilter;
    
    juce::dsp::Gain<float> inputGainProcessor;
        
    juce::dsp::Gain<float> outputGainProcessor;
    
    juce::dsp::Oversampling<float> oversamplingProcessor;
    

    juce::AlertWindow settingsDialog {"Settings Window",
            "Congrats, you opened the window, but it doesn't do anything", juce::AlertWindow::AlertIconType::InfoIcon};
    
    // Parameter listener function
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodeAmplifierAudioProcessor)
};
