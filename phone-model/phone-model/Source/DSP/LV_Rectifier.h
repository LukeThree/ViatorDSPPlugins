/*
  ==============================================================================

    LV_Rectifier.h
    Created: 30 Nov 2021 6:01:56pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#ifndef LV_Rectifier_h
#define LV_Rectifier_h

#include <JuceHeader.h>

class LV_Rectifier
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec& spec);
    
    void processBlock(juce::dsp::AudioBlock<float>& block);
    
    enum class ParameterId
    {
        kDrive,
        kBypass
    };

    void setParameter(ParameterId parameter, float parameterValue);
    
private:
    
    
    juce::SmoothedValue<float> drive {0.0};
    float currentSampleRate {44100.0f};
    float rampInSeconds {0.01};
    
    bool bypassModule;
};

#endif /* LV_Rectifier_h */
