/*
  ==============================================================================

    VoxDistortion.h
    Created: 5 May 2022 6:39:45pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template <typename SampleType>
class VoxDistortion
{
public:
    
    VoxDistortion();
    
    void prepare(juce::dsp::ProcessSpec& spec);
    
    void reset() noexcept;
    
    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        auto&& inBlock  = context.getInputBlock();
        auto&& outBlock = context.getOutputBlock();

        jassert (inBlock.getNumChannels() == outBlock.getNumChannels());
        jassert (inBlock.getNumSamples() == outBlock.getNumSamples());

        auto len         = inBlock.getNumSamples();
        auto numChannels = inBlock.getNumChannels();
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto *input = inBlock.getChannelPointer(channel);
            auto *output = outBlock.getChannelPointer(channel);
            
            for (size_t sample = 0; sample < len; ++sample)
            {
                if (mPhase)
                {
                    output[sample] = processSample(input[sample], channel) * -1.0f;
                }
                
                else
                {
                    output[sample] = processSample(input[sample], channel);
                }
            }
        }
    }
    
    SampleType processSample(SampleType newInput, int ch)
    {
        float lowBand;
        float highBand;
        
        bandFilter.setCutoffFrequency(mCutoff.getNextValue());
        bandFilter.processSample(ch, newInput, lowBand, highBand);
        
        auto highDistort = piDivisor * std::atan(highBand * mDrive.getNextValue());
        
        highDistort *= 1.75;
        
        lpFilter.setCutoffFrequency(mLPCutoff.getNextValue());
        
        auto output = lowBand + lpFilter.processSample(ch, highDistort);
        
        return ((1.0 - mMix.getNextValue()) * (lowBand + highBand)) + ((output * mMix.getNextValue()) * mTrim.getNextValue());
    }
    
    void setDrive(SampleType newDrive);
    
    void setCutoff(SampleType newCutoff);
    
    void setMix(SampleType newMix);
    
    void setLPCutoff(SampleType newCutoff);
    
    void setTrim(SampleType newTrim);
    
    void setPhase(SampleType newPhase);
    
private:
    float mSampleRate;
    juce::SmoothedValue<float> mDrive;
    juce::SmoothedValue<float> mCutoff;
    juce::SmoothedValue<float> mMix;
    juce::SmoothedValue<float> mLPCutoff;
    juce::SmoothedValue<float> mTrim;
    bool mPhase = false;
    
    static constexpr float piDivisor = 2.0 / juce::MathConstants<float>::pi;
    
    juce::dsp::LinkwitzRileyFilter<float> bandFilter;
    juce::dsp::LinkwitzRileyFilter<float> lpFilter;
};
