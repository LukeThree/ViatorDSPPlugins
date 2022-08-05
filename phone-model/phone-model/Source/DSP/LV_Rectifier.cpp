/*
  ==============================================================================

    LV_Rectifier.cpp
    Created: 30 Nov 2021 6:01:56pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_Rectifier.h"

void LV_Rectifier::reset()
{
    if (currentSampleRate > 0)
    {
        drive.reset(currentSampleRate, rampInSeconds);
    }
}

void LV_Rectifier::prepare(juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    reset();
}

void LV_Rectifier::processBlock(juce::dsp::AudioBlock<float>& block)
{
    // Disable circuit
    if (bypassModule) return;
    
    // Process circuit
    for (int sample = 0; sample < block.getNumSamples(); ++sample)
    {
        for (int ch = 0; ch < block.getNumChannels(); ++ch)
        {
            float* data = block.getChannelPointer(ch);
            
            if (data[sample] < 0)
            {
                data[sample] *= drive.getNextValue();
            }
        }
    }
}

void LV_Rectifier::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case LV_Rectifier::ParameterId::kDrive:
        {
            drive.setTargetValue(parameterValue);
            break;
        }
            
        case LV_Rectifier::ParameterId::kBypass:
        {
            bypassModule = parameterValue;
            break;
        }
    }
}
