/*
  ==============================================================================

    Stereo-Widener.h
    Created: 16 Dec 2021 7:01:09pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#ifndef LV_Widener_h
#define LV_Widener_h

#include <JuceHeader.h>

class LV_Widener
{
public:
    
    void prepare(juce::dsp::ProcessSpec& spec);
    
    void processBlock(juce::dsp::AudioBlock<float>& block);
    
    enum class ParameterId
    {
        kLowWidth,
        kHighWidth,
        kCrossover,
        kBypass
    };

    void setParameter(ParameterId parameter, float parameterValue);
    
private:
    
    void assertRange(float sourceValue, float min, float max);
    void assertBool(float sourceValue);
    
    juce::SmoothedValue<float> lowWidth {1.0f};
    juce::SmoothedValue<float> highWidth {1.0f};
    juce::SmoothedValue<float> crossoverFreq {10000.0f};
    
    bool bypassModule {false};
    float currentSampleRate;
    
    juce::dsp::LinkwitzRileyFilter<float> crossoverFilterModule;
    juce::dsp::LinkwitzRileyFilter<float> crossoverFilterModule2;
    juce::dsp::LinkwitzRileyFilter<float> crossoverFilterModule3;
    juce::dsp::LinkwitzRileyFilter<float> crossoverFilterModule4;
};

#endif /* LV_Widener_h */
