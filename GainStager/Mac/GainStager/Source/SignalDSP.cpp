/*
  ==============================================================================

    GetSignals.cpp
    Created: 10 Jun 2021 1:14:07pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

float GainStagerAudioProcessor::getPeakSignal(std::vector<float> vec, float rms)
{
    for (auto i{ 0 }; i < vec.size(); i++)
    {
        if (vec[i] > rms)
        {
            rms = vec[i];
        }
    }
        
    return juce::Decibels::gainToDecibels(rms);
}

float GainStagerAudioProcessor::getAverageSignal(std::vector<float> vec, float rms)
{
    auto acc {0.0f};
    auto localPeak {0.0f};

    for (auto i{ 0 }; i < vec.size(); i++)
    {
        if (vec[i] > rms)
        {
            rms = vec[i];
            localPeak = vec[i];
        }
        
        auto sample = vec[i];
        
        acc += sample * sample;
    }
    
    auto avg = acc / vec.size();
    
    if (avg > 0.0)
    {
        rms = std::sqrt (avg);
    }

    auto average = (juce::Decibels::gainToDecibels(rms) + juce::Decibels::gainToDecibels(localPeak)) / 2.0f;

    return average;
}

