/*
  ==============================================================================

    Stereo-Widener.cpp
    Created: 16 Dec 2021 7:01:09pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_Widener.h"

void LV_Widener::prepare(juce::dsp::ProcessSpec &spec)
{
    currentSampleRate = spec.sampleRate;
    
    lowWidth.reset(currentSampleRate, 0.02);
    crossoverFreq.reset(currentSampleRate, 0.02);
    
    crossoverFilterModule.prepare(spec);
    crossoverFilterModule.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    crossoverFilterModule2.prepare(spec);
    crossoverFilterModule2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    crossoverFilterModule3.prepare(spec);
    crossoverFilterModule3.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    crossoverFilterModule4.prepare(spec);
    crossoverFilterModule4.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
}

void LV_Widener::processBlock(juce::dsp::AudioBlock<float> &block)
{
    if (bypassModule) return;
    
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        // Getting the left and right channels into variables
        float* leftData = block.getChannelPointer(0);
        float* rightData;
        
        // Check for our IO setting
        if (block.getNumChannels() == 1)
        {
            rightData = block.getChannelPointer(0);
        }
        
        if (block.getNumChannels() == 2)
        {
            rightData = block.getChannelPointer(1);
        }
        
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            const auto lowMidRaw = 0.5 * (leftData[sample] + rightData[sample]);
            const auto lowSidesRaw = 0.5 * (leftData[sample] - rightData[sample]);
            
            crossoverFilterModule.setCutoffFrequency(crossoverFreq.getNextValue());
            crossoverFilterModule2.setCutoffFrequency(crossoverFreq.getNextValue());
            crossoverFilterModule3.setCutoffFrequency(crossoverFreq.getNextValue());
            crossoverFilterModule4.setCutoffFrequency(crossoverFreq.getNextValue());
            
            const auto lowMid = crossoverFilterModule.processSample(channel, lowMidRaw);
            const auto lowSides = crossoverFilterModule2.processSample(channel, lowSidesRaw);
            
            const auto newLowMid = (2.0 - lowWidth.getNextValue()) * (lowMid);
            const auto newLowSides = lowWidth.getNextValue() * lowSides;
            
            const auto newLowLeft = newLowMid + newLowSides;
            const auto newLowRight = newLowMid - newLowSides;
            
            const auto highMid = crossoverFilterModule3.processSample(channel, lowMidRaw);
            const auto highSides = crossoverFilterModule4.processSample(channel, lowSidesRaw);
            
            const auto newHighMid = (2.0 - highWidth.getNextValue()) * (highMid);
            const auto newhighSides = highWidth.getNextValue() * highSides;
            
            const auto newHighLeft = newHighMid + newhighSides;
            const auto newHighRight = newHighMid - newhighSides;
            
            const auto newLowWidth = juce::jmap(lowWidth.getNextValue(), 0.0f, 2.0f, -1.0f, 1.0f);
            const auto newHighWidth = juce::jmap(highWidth.getNextValue(), 0.0f, 2.0f, -1.0f, 1.0f);
            
            float lowVolumeScale = 0.0;
            float highVolumeScale = 0.0;
            
            if (newLowWidth < 0.0)
            {
                lowVolumeScale = newLowWidth * 12.0;
            }
            
            if (newHighWidth < 0.0)
            {
                highVolumeScale = newHighWidth * 3.0;
            }
            
            const auto scaledLow = juce::Decibels::decibelsToGain(lowVolumeScale);
            const auto scaledHigh = juce::Decibels::decibelsToGain(highVolumeScale);

            leftData[sample] = newLowLeft * scaledLow + newHighLeft * scaledHigh;
            rightData[sample] = newLowRight * scaledLow + newHighRight * scaledHigh;
            
            }
        }
    }

void LV_Widener::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case ParameterId::kLowWidth:
        {
            lowWidth.setTargetValue(parameterValue);
            break;
        }
            
        case ParameterId::kHighWidth:
        {
            highWidth.setTargetValue(parameterValue);
            break;
        }
            
        case ParameterId::kCrossover:
        {
            crossoverFreq.setTargetValue(parameterValue);
            break;
        }
            
        case ParameterId::kBypass:
        {
            bypassModule = parameterValue;
            break;
        }
    }
}

void LV_Widener::assertBool(float sourceValue)
{
    
}

void LV_Widener::assertRange(float sourceValue, float min, float max)
{
    
}
