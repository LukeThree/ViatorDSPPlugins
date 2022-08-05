/*
  ==============================================================================

    LV_Clipper.h
    Created: 12 Jan 2022 3:55:22pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#ifndef LV_Clipper_h
#define LV_Clipper_h

#include <JuceHeader.h>
#include "LV_SVFilter.h"

class LV_Clipper
{
public:
    
    void prepare(juce::dsp::ProcessSpec& spec);
        
    void processBlock(juce::dsp::AudioBlock<float>& block);
        
    float returnSoftClip(const float input);
        
    float returnHardClip(const float input, const int channel);
        
    float returnAnalogClip(const float input, const int channel);
    
    enum class ParameterId
    {
        kDrive,
        kCeiling,
        kMix,
        kTrim,
        kBypass
    };
    
    enum class ClipperTypeId
    {
        kSoftClipper,
        kHardClipper,
        kAnalogClipper
    };

    void setParameter(ParameterId parameter, float parameterValue);
    
    void setClipperType(ClipperTypeId newType);
    
private:
    
    float logScale(float value, float scalar = 0.05f) const;
    
    juce::SmoothedValue<float> drive {0.0f};
    juce::SmoothedValue<float> ceiling {0.0f};
    juce::SmoothedValue<float> trim {0.0f};
    juce::SmoothedValue<float> mix {0.0f};
    
    float sampleRate {44100.0f};
    float clippedSignal {0.0f};
    float blendSignal {0.0f};
    float drySignal {0.0f};
    
    float softCoeffA {1.5};
    float softCoeffB {-0.5};
    
    float analogCoeffA {0.1};
    float analogCoeffB {8.0};
    float analogCoeffC {0.0253};
    
    bool bypassModule {false};
    
    const float piDivisor = 2.0f / juce::MathConstants<float>::pi;
    
    ClipperTypeId clipperType = ClipperTypeId::kSoftClipper;
};

#endif /* LV_Clipper_h */
