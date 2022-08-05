#ifndef GraphicEQ_h
#define GraphicEQ_h

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class GraphicEQ
{
public:
    
    GraphicEQ();
    
    /** Initialises the clipper. */
    void prepare(const juce::dsp::ProcessSpec& spec);
    
    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        filter1.process(context);
        filter2.process(context);
        filter3.process(context);
        filter4.process(context);
        filter5.process(context);
        filter6.process(context);
        filter7.process(context);
        filter8.process(context);
        filter9.process(context);
        filter10.process(context);
    }
    
    /** The parameters of this module. */
    enum class ParameterId
    {
        kFilter1Gain,
        kFilter2Gain,
        kFilter3Gain,
        kFilter4Gain,
        kFilter5Gain,
        kFilter6Gain,
        kFilter7Gain,
        kFilter8Gain,
        kFilter9Gain,
        kFilter10Gain,
        kSampleRate,
        kBypass
    };
    
    void setParameter(ParameterId parameter, SampleType parameterValue);
    void setStereoType(int newStereoType);
    
private:
    
    // Member variables
    bool mGlobalBypass;
    juce::SmoothedValue<float> mGainDB;
    juce::SmoothedValue<float> mRawGain;
    float mCurrentSampleRate {44100.0f};
    
    // Expressions
    static constexpr float mPiDivisor = 2.0 / juce::MathConstants<float>::pi;
    
    // Filters
    viator_dsp::SVFilter<float> filter1;
    viator_dsp::SVFilter<float> filter2;
    viator_dsp::SVFilter<float> filter3;
    viator_dsp::SVFilter<float> filter4;
    viator_dsp::SVFilter<float> filter5;
    viator_dsp::SVFilter<float> filter6;
    viator_dsp::SVFilter<float> filter7;
    viator_dsp::SVFilter<float> filter8;
    viator_dsp::SVFilter<float> filter9;
    viator_dsp::SVFilter<float> filter10;
};

#endif /* Distortion_h */
