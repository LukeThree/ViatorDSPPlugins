/*
  ==============================================================================

    LV_HardClipper.h
    Created: 8 Jul 2021 1:35:11pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#ifndef LV_HardClipper_h
#define LV_HardClipper_h

#include <JuceHeader.h>

class LV_HardClipper
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    float processSample(float input);
    
    enum class ParameterId {
            kPreamp,
            kThresh,
            kTrim,
            kBlend,
            kSampleRate
        };

    
    void setParameter(ParameterId parameter, float parameterValue);
    
    float getPreamp();
    
    float getThresh();
    
    float getTrim();
    
    float getBlend();
    
    float getSampleRate();
    
    bool getPowerState();
    
private:
    
    float preamp {1.0}, thresh {1.0}, trim {1.0}, blend {1.0}, currentSampleRate {44100.0}, output {0.0};
    
    bool powerState = true;
};

#endif /* LV_HardClipper_h */
