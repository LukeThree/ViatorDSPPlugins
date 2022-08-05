/*
  ==============================================================================

    LV_TubeModel.cpp
    Created: 13 Jul 2021 11:40:01pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_TubeModel.h"

void LV_TubeModel::reset()
{
}

void LV_TubeModel::prepare(juce::dsp::ProcessSpec spec)
{
    currentSampleRate = spec.sampleRate;
    
    if (preamp == 0.0)
    {
        powerState = false;
    }
    
    else
    {
        powerState = true;
    }
}

float LV_TubeModel::processSample(float input, int ch)
{
    auto x = input * juce::Decibels::decibelsToGain(9.0f);
    
    const float driveScaled = pow(10.0f, preamp / 20.0f);
                
    const float circleMap = x + (driveScaled / juce::MathConstants<float>::twoPi) * std::sin(juce::MathConstants<float>::twoPi * x);
                    
    auto mix = (x * (1.0 - 0.5f) + (circleMap * 0.5f));
                
    mix *= 1.1f;
                
    mix *= std::powf(10.0f, -driveScaled * 0.08);
                
    return mix * juce::Decibels::decibelsToGain(-9.0f);
}





void LV_TubeModel::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter) {
            
        case ParameterId::kPreamp:
        {
            preamp = parameterValue;
            break;
        }
            
        case ParameterId::kDrive:
        {
            // Should be in dB
            drive = parameterValue; break;
        }
            
        case ParameterId::kTrim:
        {
            // Should be in dB
            trim = parameterValue; break;
        }
            
        case ParameterId::kBlend:
        {
            // Should be in %
            blend = juce::jmap(parameterValue, 0.0f, 100.0f, 0.0f, 1.0f); break;
        }
            
        case ParameterId::kSampleRate:
        {
            currentSampleRate = parameterValue; break;
        }
    }
}

float LV_TubeModel::getPreamp()
{
    return preamp;
}

float LV_TubeModel::getDrive()
{
    return drive;
}

float LV_TubeModel::getTrim()
{
    return trim;
}

float LV_TubeModel::getBlend()
{
    return blend;
}

float LV_TubeModel::getSampleRate()
{
    return currentSampleRate;
}

bool LV_TubeModel::getPowerState()
{
    return powerState;
}
