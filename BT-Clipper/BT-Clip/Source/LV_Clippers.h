/*
  ==============================================================================

    LV_SoftClipper.h
    Created: 8 Jul 2021 3:08:32pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#ifndef LV_Clippers_h
#define LV_Clippers_h

#include <JuceHeader.h>

class LV_Clippers
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    float processSample(float input);
    
    enum class ParameterId {
            kPreamp,
            kBias,
            kDrive,
            kTrim,
            kBlend,
            kSampleRate
        };
    
    enum class ClippingType
    {
        kHardClip,
        kSoftClip,
        kAnalog
    };
    
    void setParameter(ParameterId parameter, float parameterValue);
    
    void set_clipping_type(ClippingType clippingType);
    
    
    float getPreamp();
    
    float getBias();
    
    float getTrim();
    
    float getBlend();
    
    float getSampleRate();
    
    bool getPowerState();
    
private:
    
    float preamp {1.0}, trim {1.0}, blend {1.0}, currentSampleRate {44100.0}, output {0.0}, drive{1.0f}, piDivisor, bias, compensate {1.0}, thresh {1.0};
    
    int clipperType {0};
    
    bool powerState = true;
    
    ClippingType m_ClipType = ClippingType::kHardClip;
};

#endif /* LV_Clippers_h */
