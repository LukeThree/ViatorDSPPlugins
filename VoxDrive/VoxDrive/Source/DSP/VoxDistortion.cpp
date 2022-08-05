/*
  ==============================================================================

    VoxDistortion.cpp
    Created: 5 May 2022 6:39:45pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "VoxDistortion.h"

template <typename SampleType>
VoxDistortion<SampleType>::VoxDistortion()
{
    
}

template <typename SampleType>
void VoxDistortion<SampleType>::prepare(juce::dsp::ProcessSpec &spec)
{
    mSampleRate = spec.sampleRate;
    reset();
    
    bandFilter.prepare(spec);
    bandFilter.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    bandFilter.setCutoffFrequency(mCutoff.getNextValue());
    
    lpFilter.prepare(spec);
    lpFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lpFilter.setCutoffFrequency(mLPCutoff.getNextValue());
}

template <typename SampleType>
void VoxDistortion<SampleType>::reset() noexcept
{
    if (mSampleRate > 0)
    {
        mDrive.reset(mSampleRate, 0.02);
        mDrive.setTargetValue(0.0);
        setDrive(mDrive.getNextValue());
        
        mCutoff.reset(mSampleRate, 0.02);
        mCutoff.setTargetValue(500.0f);
        
        mMix.reset(mSampleRate, 0.02);
        mMix.setTargetValue(1.0f);
        
        mLPCutoff.reset(mSampleRate, 0.02);
        mLPCutoff.setTargetValue(1000.0f);
        
        mTrim.reset(mSampleRate, 0.02);
        mTrim.setTargetValue(0.0f);
    }
}

template <typename SampleType>
void VoxDistortion<SampleType>::setDrive(SampleType newDrive)
{
    mDrive.setTargetValue(juce::Decibels::decibelsToGain(newDrive));
}

template <typename SampleType>
void VoxDistortion<SampleType>::setCutoff(SampleType newCutoff)
{
    mCutoff.setTargetValue(newCutoff);
}

template <typename SampleType>
void VoxDistortion<SampleType>::setMix(SampleType newMix)
{
    auto mixScaled = juce::jmap(static_cast<float>(newMix), 0.0f, 100.0f, 0.0f, 1.0f);
    mMix.setTargetValue(mixScaled);
}

template <typename SampleType>
void VoxDistortion<SampleType>::setLPCutoff(SampleType newCutoff)
{
    mLPCutoff.setTargetValue(newCutoff);
}

template <typename SampleType>
void VoxDistortion<SampleType>::setTrim(SampleType newTrim)
{
    mTrim.setTargetValue(juce::Decibels::decibelsToGain(newTrim));
}

template <typename SampleType>
void VoxDistortion<SampleType>::setPhase(SampleType newPhase)
{
    mPhase = newPhase;
}

template class VoxDistortion<float>;
template class VoxDistortion<double>;
