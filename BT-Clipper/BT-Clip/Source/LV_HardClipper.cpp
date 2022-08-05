/*
  ==============================================================================

    LV_HardClipper.cpp
    Created: 8 Jul 2021 1:35:11pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_HardClipper.h"

void LV_HardClipper::reset()
{
    preamp = 1.0;
    thresh = 1.0;
    trim = 1.0;
    blend = 1.0;
    powerState = true;
}

void LV_HardClipper::prepare(juce::dsp::ProcessSpec spec)
{
    currentSampleRate = spec.sampleRate;
}

float LV_HardClipper::processSample(float input)
{
    // Don't process signal if the module isn't enabled
    if (!powerState) return input;
    
    // Handy variables
    auto x_n = input * preamp;
         
    // Hard clipper
    if (x_n >= thresh)
    {
        output = thresh;
    }
        
    else if (x_n <= -thresh)
    {
        output = -thresh;
    }
        
    else
    {
        output = x_n;
    }
    
    // Blend the hard clipper with the input
    auto hardClip = x_n * (1.0 - blend) + output * blend;
    
    // Output the blended signal
    return hardClip * trim;
}

void LV_HardClipper::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter) {
            
        case ParameterId::kPreamp:
        {
            // Should be in dB
            preamp = pow(10.0, parameterValue / 20.0); break;
        }
            
        case ParameterId::kThresh:
        {
            // Should be in dB
            thresh = pow(10.0, parameterValue / 20.0); break;
        }
            
        case ParameterId::kTrim:
        {
            // Should be in dB
            trim = pow(10.0, parameterValue / 20.0); break;
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

float LV_HardClipper::getPreamp()
{
    return preamp;
}

float LV_HardClipper::getThresh()
{
    return thresh;
}

float LV_HardClipper::getTrim()
{
    return trim;
}

float LV_HardClipper::getBlend()
{
    return blend;
}

float LV_HardClipper::getSampleRate()
{
    return currentSampleRate;
}

bool LV_HardClipper::getPowerState()
{
    return powerState;
}
