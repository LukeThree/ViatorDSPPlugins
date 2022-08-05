/*
  ==============================================================================

    FastClipperModule.cpp
    Created: 2 Mar 2022 6:19:57pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "FastClipperModule.h"

viator_dsp::FastClipperModule::FastClipperModule() :
mGlobalBypass(false), mThresh(1.0f), mGainDB(1.0), mClipType(viator_dsp::FastClipperModule::ClipType::kHard)
{
    mPiDivisor = 2.0 / juce::MathConstants<float>::pi;
}

void viator_dsp::FastClipperModule::prepare(const juce::dsp::ProcessSpec& spec)
{
    mCurrentSampleRate = spec.sampleRate;
    mRawGain.reset(mCurrentSampleRate, 0.02);
    mRawGain.setTargetValue(0.0);
}

void viator_dsp::FastClipperModule::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case ParameterId::kPreamp:
        {
            mRawGain.setTargetValue(parameterValue);
            mGainDB = juce::Decibels::decibelsToGain(mRawGain.getNextValue()); break;
        }
        case ParameterId::kSampleRate: mCurrentSampleRate = parameterValue; break;
        case ParameterId::kThresh: mThresh = parameterValue; break;
        case ParameterId::kBypass: mGlobalBypass = static_cast<bool>(parameterValue);
    }
}

void viator_dsp::FastClipperModule::setClipperType(ClipType clipType)
{
    switch (clipType)
    {
        case ClipType::kHard: mClipType = viator_dsp::FastClipperModule::ClipType::kHard; break;
        case ClipType::kSoft: mClipType = viator_dsp::FastClipperModule::ClipType::kSoft; break;
        case ClipType::kDiode: mClipType = viator_dsp::FastClipperModule::ClipType::kDiode; break;
    }
}

juce::dsp::SIMDRegister<float> viator_dsp::FastClipperModule::hardClipData(juce::dsp::SIMDRegister<float> dataToClip, const float thresh, int sampleIndex)
{
    /** Don't do anything if the module is off*/
    if (mGlobalBypass) return dataToClip;
    
    /** Hard Clipping algorithim*/
    if (std::abs(dataToClip.get(sampleIndex)) > thresh)
    {
        dataToClip *= thresh / std::abs(dataToClip.get(sampleIndex));
    }
    
    return dataToClip;
}

juce::dsp::SIMDRegister<float> viator_dsp::FastClipperModule::softClipData(juce::dsp::SIMDRegister<float> dataToClip, int sampleIndex)
{
    /** Don't do anything if the module is off*/
    if (mGlobalBypass) return dataToClip;
    
    /** Soft Clipping algorithim*/
    auto softClipper = mPiDivisor * std::atan(dataToClip.get(sampleIndex)) * 2.0;
    
    /** Auto gain stage the soft clipper*/
    softClipper *= viator_utils::utils::dbToGain(-mRawGain.getNextValue() * 0.25);
    
    return softClipper;
}

juce::dsp::SIMDRegister<float> viator_dsp::FastClipperModule::diodeClipper(juce::dsp::SIMDRegister<float> dataToClip, int sampleIndex)
{
    /** Don't do anything if the module is off*/
    if (mGlobalBypass) return dataToClip;
    
    /** Diode Clipping algorithim*/
    auto diode = 0.105 * (juce::dsp::FastMathApproximations::exp(0.1 * dataToClip.get(sampleIndex) / (2.0 * 0.0253)) - 1.0);
    diode -= 0.28;
    diode *= 3.0;
    
    return softClipData(diode, sampleIndex);
}
