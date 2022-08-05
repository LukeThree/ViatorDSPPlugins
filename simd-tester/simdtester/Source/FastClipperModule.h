/*
  ==============================================================================

    FastClipperModule.h
    Created: 2 Mar 2022 6:19:57pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#ifndef Fast_Clipper_Module_h
#define Fast_Clipper_Module_h

namespace viator_dsp
{
class FastClipperModule
{
public:
    
    /** Creates an uninitialised clipper. Call prepare() before first use. */
    FastClipperModule();
    
    /** Initialises the clipper. */
    void prepare(const juce::dsp::ProcessSpec& spec);
    
    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        if (mGlobalBypass)
        {
            return;
        }
        
        auto&& inBlock  = context.getInputBlock();
        auto&& outBlock = context.getOutputBlock();

        jassert (inBlock.getNumChannels() == outBlock.getNumChannels());
        jassert (inBlock.getNumSamples() == outBlock.getNumSamples());

        auto len         = inBlock.getNumSamples();
        auto numChannels = inBlock.getNumChannels();


        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            for (size_t sample = 0; sample < len; ++sample)
            {
                auto* input = inBlock.getChannelPointer (channel);
                auto* output = outBlock.getChannelPointer (channel);
                
                auto x = input[sample] * viator_utils::utils::dbToGain(mRawGain.getNextValue());
                output[sample] = processSample(x, sample);
            }
        }
    }
    
    /** Process an individual sample */
    juce::dsp::SIMDRegister<float> processSample(juce::dsp::SIMDRegister<float> input, int sampleIndex) noexcept
    {
        switch(mClipType)
        {
            case ClipType::kHard: return hardClipData(input.get(sampleIndex) * mGainDB, mThresh, sampleIndex); break;
            case ClipType::kSoft: return softClipData(input.get(sampleIndex) * mGainDB, sampleIndex); break;
            case ClipType::kDiode: return diodeClipper(input.get(sampleIndex) * mGainDB, sampleIndex); break;
        }
    }
    
    /** The parameters of this module. */
    enum class ParameterId
    {
        kPreamp,
        kSampleRate,
        kThresh,
        kBypass
    };
    
    /** Different clipper types*/
    enum class ClipType
    {
        kHard,
        kSoft,
        kDiode
    };
        
    /** One method to change any parameter. */
    void setParameter(ParameterId parameter, float parameterValue);
    void setClipperType(ClipType clipType);
    
private:
    
    // Member variables
    bool mGlobalBypass;
    juce::SmoothedValue<float> mRawGain;
    float mCurrentSampleRate, mThresh, mPiDivisor, mGainDB;
    
    // Methods
    juce::dsp::SIMDRegister<float> hardClipData(juce::dsp::SIMDRegister<float> dataToClip, const float thresh, int sampleIndex);
    juce::dsp::SIMDRegister<float> softClipData(juce::dsp::SIMDRegister<float> dataToClip, int sampleIndex);
    juce::dsp::SIMDRegister<float> diodeClipper(juce::dsp::SIMDRegister<float> dataToClip, int sampleIndex);
    
    ClipType mClipType;
};
} // namespace viator_dsp

#endif /* Fast_Clipper_Module_h */
