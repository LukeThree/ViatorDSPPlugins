/*
  ==============================================================================

    LV_EQ.cpp
    Created: 25 May 2021 5:40:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_SVFilter.h"

void LV_SVFilter::reset()
{
    
}
//==============================================================================

void LV_SVFilter::prepare(juce::dsp::ProcessSpec spec)
{
    mCurrentSampleRate = spec.sampleRate;
    mCutoff = 5.0f;
    mQ = 0.1f;
    mGain = convertToGain(0.0);
    mType = kHighPass;
    mQType = kParametric;
    mRawGain = 0.0f;
    mGlobalBypass = false;
    
    mZ1.assign(spec.numChannels, 0.0);
    mZ2.assign(spec.numChannels, 0.0);
}
//==============================================================================

void LV_SVFilter::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case ParameterId::kType: mType = (FilterType)parameterValue; break;
        case ParameterId::kQType: mQType = (QType)parameterValue; break;
        case ParameterId::kCutoff:
        {
            mCutoff = parameterValue;
            
            if (parameterValue == 20000.0)
            {
                mGlobalBypass = true;
            }

            else
            {
                mGlobalBypass = buttonState;
            }
            
            break;
        }
            
        case ParameterId::kQ: mQ = parameterValue; break;
        case ParameterId::kGain:
        {
            setGain(parameterValue);
            
            if (parameterValue == 0.0)
            {
                mGlobalBypass = true;
            }

            else
            {
                mGlobalBypass = buttonState;
            }
            
            break;
        }
            
        case ParameterId::kSampleRate: mCurrentSampleRate = parameterValue; break;
        case ParameterId::kBypass:
        {
            mGlobalBypass = static_cast<bool>(parameterValue);
            buttonState = mGlobalBypass; break;
        }
    }
}
//==============================================================================

void LV_SVFilter::setGain(float value)
{
    mGain = convertToGain(value);
    mRawGain = value;
}
//==============================================================================

float LV_SVFilter::convertToGain(const float value) const
{
    constexpr double inversion = 1.0 / 20.0;
    return pow(10, value * inversion) - 1.f;
}
//==============================================================================

float LV_SVFilter::getShelfQ(float value) const
{
    if (value <= 12.0f && value >= -12.0f)
    {
        return pow(10.0f, abs(value) / 80.0f) - 0.8f;
    }
    
    else
    {
        return 0.69f;
    }
}
//==============================================================================

float LV_SVFilter::getPeakQ(float value) const
{
    // Outside of these ranges would break the DSP that controls the Q
    if (value <= 12.0f && value >= -12.0f)
    {
        if (value > 0.0f)
        {
            return pow(10.0f, abs(value) / 60.0f) - 0.9f;
        }
        
        else
        {
            return pow(10.0f, abs(value) / 45.0f) - 0.9f;
        }
    }
    
    else
    {
        
        if (value > 12.0f)
        {
            return  0.79f;
        }
        
        else
        {
            return 0.95f;
        }
    }
}
//==============================================================================

float LV_SVFilter::processSample(float input, int ch)
{
    if (mGlobalBypass) return input;
        
    float lsLevel = 0.0;
    float bsLevel = 0.0;
    float hsLevel = 0.0;
    float lpLevel = 0.0;
    float hpLevel = 0.0;
        
    switch (mType)
    {
        case kLowShelf: lsLevel = 1.0; break;
        case kBandShelf: bsLevel = 1.0; break;
        case kHighShelf: hsLevel = 1.0; break;
        case kLowPass: lpLevel = 1.0; break;
        case kHighPass: hpLevel = 1.0; break;
    }
        
    const double sampleRate2X = mCurrentSampleRate * 2.0;
    const double halfSampleDuration = 1.0 / mCurrentSampleRate / 2.0;
        
    // prewarp the cutoff (for bilinear-transform filters)
    double wd = mCutoff * 6.28f;
    double wa = sampleRate2X * tan(wd * halfSampleDuration);
                
    //Calculate g (gain element of integrator)
    mGCoeff = wa * halfSampleDuration;
                
    //Calculate Zavalishin's damping parameter (Q)
    switch (mQType)
    {
        case kParametric: mRCoeff = 1.0 - mQ; break;
        case kProportional:
        {
            if (mType == kBandShelf)
            {
                mRCoeff = 1.0 - getPeakQ(mRawGain); break;
            }
                
            else
            {
                mRCoeff = 1.0 - getShelfQ(mRawGain); break;
            }
        }
    }
        
    mRCoeff2 = mRCoeff * 2.0;
                
    mInversion = 1.0 / (1.0 + mRCoeff2 * mGCoeff + mGCoeff * mGCoeff);
            
    const auto z1 = mZ1[ch];
    const auto z2 = mZ2[ch];
                            
    const double HP = (input - mRCoeff2 * z1 - mGCoeff * z1 - z2) * mInversion;
    const double BP = HP * mGCoeff + z1;
    const double LP = BP * mGCoeff + z2;
    const double UBP = mRCoeff2 * BP;
    const double BShelf = input + UBP * mGain;
    const double LS = input + mGain * LP;
    const double HS = input + mGain * HP;
                
    //Main output code
    input = BShelf * bsLevel + LS * lsLevel + HS * hsLevel + HP * hpLevel + LP * lpLevel;
               
    // unit delay (state variable)
    mZ1[ch] = mGCoeff * HP + BP;
    mZ2[ch] = mGCoeff * BP + LP;
    
    return input;
}
