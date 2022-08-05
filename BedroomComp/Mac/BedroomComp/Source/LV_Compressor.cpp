/*
  ==============================================================================

    LV_Compressor.cpp
    Created: 29 Jun 2021 12:43:26am
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_Compressor.h"

void LV_Compressor::reset()
{
    
}

void LV_Compressor::prepare(juce::dsp::ProcessSpec spec)
{
    currentSampleRate = spec.sampleRate;
}

float LV_Compressor::processSample(float input, float hpSample, bool hpSC)
{
    // Don't process signal is the compressor isn't enabled
    if (!enabled)
        return input;
    
    /* Initialise separate attack/release times, might need to do sampleRate / buffer.getNumSamples * time*/
    auto alphaAttack = exp(-log(9) / (currentSampleRate * attack));
    auto alphaRelease = exp(-log(9) / (currentSampleRate * release));
    
    const auto x = input;
    const auto xHP = hpSample;

    auto x_Uni = abs(x);
    auto x_dB = juce::Decibels::gainToDecibels(x_Uni);

    auto x_Uni_HP = abs(xHP);
    auto x_dB_HP = juce::Decibels::gainToDecibels(x_Uni_HP);
    
    if (x_dB < -96.0)
    {
        x_dB = -96.0;
    }
    
    auto x_dBU = x_dB + 18.0;
    auto x_dBU_HP = x_dB_HP + 18.0;

    /**Do we use the hp signal or regular signal for the threshold check*/
    if (hpSC)
    {
        if (x_dBU_HP > thresh)
        {
            gainSC = thresh + (x_dBU - thresh) / ratio;
        }

        else
        {
            gainSC = x_dBU;
        }
    }
    
    else
    {
        if (x_dBU > thresh)
        {
            gainSC = thresh + (x_dBU - thresh) / ratio;
        }

        else
        {
            gainSC = x_dBU;
        }
    }

    gainChange_dB = gainSC - x_dBU;

    if (gainChange_dB < gainSmoothPrevious)
    {
        gainSmooth = ((1 - alphaAttack) * gainChange_dB) + (alphaAttack * gainSmoothPrevious);
        currentSignal = gainSmooth;
    }

    else
    {
        gainSmooth = ((1 - alphaRelease) * gainChange_dB) + (alphaRelease * gainSmoothPrevious);
        currentSignal = gainSmooth;
    }
    
    gainSmoothPrevious = gainSmooth;

    // Output the compressed signal plus the dry signal, scaled with the blend knob
    return ((1.0 - scaledBlend) * x + (x * juce::Decibels::decibelsToGain(gainSmooth) * scaledBlend)) * scaledMakeUp;
}

void LV_Compressor::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter) {
            
        case ParameterId::kThresh: thresh = parameterValue; break;
        case ParameterId::kRatio: ratio = ratioValues[parameterValue]; break;
        case ParameterId::kAttack: attack = parameterValue; break;
        case ParameterId::kRelease: release = parameterValue; break;
        case ParameterId::kSampleRate: currentSampleRate = parameterValue; break;

        case ParameterId::kBlend:
        {
            blend = parameterValue;
            scaledBlend = juce::jmap(blend, 0.0f, 100.0f, 0.0f, 1.0f); break;
        }
            
        case ParameterId::kMakeUp:
        {
            makeUp = parameterValue;
            scaledMakeUp = juce::Decibels::decibelsToGain(makeUp); break;
        }
            
        case ParameterId::kEnabled: enabled = parameterValue; break;
        case ParameterId::kSpeedValueStringsVec: speedDialString = speedValueStrings[parameterValue]; break;
            
        case ParameterId::kTwoDSpeedValues:
        {
            attack = twoDSpeedValues[parameterValue][0] / 1000.0;
            release = twoDSpeedValues[parameterValue][1] / 1000.0; break;
        }

    }
}

float LV_Compressor::getCurrentSignal()
{
    return currentSignal;
}

float LV_Compressor::getBlend()
{
    return blend;
}

float LV_Compressor::getMakeUp()
{
    return makeUp;
}

float LV_Compressor::getRatio()
{
    return ratio;
}

float LV_Compressor::getThresh()
{
    return thresh;
}


float LV_Compressor::getAtttack()
{
    return attack;
}

float LV_Compressor::getRelease()
{
    return release;
}

float LV_Compressor::getScaledBlend()
{
    return scaledBlend;
}

float LV_Compressor::getScaledMakeUp()
{
    return scaledMakeUp;
}

std::string LV_Compressor::getSpeedValueString()
{
    return speedDialString;
}
