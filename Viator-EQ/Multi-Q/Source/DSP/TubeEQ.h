#ifndef TubeEQ_h
#define TubeEQ_h

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class TubeEQ
{
public:
    
    TubeEQ();
    
    /** Initialises the clipper. */
    void prepare(const juce::dsp::ProcessSpec& spec);
    
    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        
        auto&& inBlock  = context.getInputBlock();
        auto&& outBlock = context.getOutputBlock();

        jassert (inBlock.getNumChannels() == outBlock.getNumChannels());
        jassert (inBlock.getNumSamples() == outBlock.getNumSamples());

        auto len         = inBlock.getNumSamples();
        auto numChannels = inBlock.getNumChannels();

        lowBoostFilter.process(context);
        lowCutFilter.process(context);
        highBoostFilter.process(context);
        highCutFilter.process(context);
        
        if (mRawGain.getNextValue() == 1.0) return;
        
        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* input = inBlock.getChannelPointer(ch);
            auto* output = outBlock.getChannelPointer (ch);
                    
            for (int sample = 0; sample < len; ++sample)
            {
                output[sample] = tubeDistortion(input[sample]);
            }
        }
    }
    
    SampleType tubeDistortion(SampleType dataToClip)
    {
        // Preamp
        dataToClip *= mRawGain.getNextValue();
        
        dataToClip += 0.25;
            
        // Tube logic
        if (dataToClip < 0.0)
        {
            dataToClip = mPiDivisor * std::atan(dataToClip);
        }
                
        else
        {
            if (std::abs(dataToClip) >= 0.99)
            {
                dataToClip = std::copysign(0.99, dataToClip);
            }
        }
        
        dataToClip -= 0.25;
            
            // Output
        return dataToClip *= viator_utils::utils::dbToGain(-mGainDB.getNextValue() * 0.75);
        
    }

    /** The parameters of this module. */
    enum class ParameterId
    {
        kLowBoostGain,
        kLowCutGain,
        kHighBoostGain,
        kHighCutGain,
        kLowFilterFreq,
        kHighFilterFreq,
        kBandWidth,
        kDrive,
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
    viator_dsp::SVFilter<float> lowBoostFilter;
    viator_dsp::SVFilter<float> lowCutFilter;
    viator_dsp::SVFilter<float> highBoostFilter;
    viator_dsp::SVFilter<float> highCutFilter;
};

#endif /* TubeEQ_h */
