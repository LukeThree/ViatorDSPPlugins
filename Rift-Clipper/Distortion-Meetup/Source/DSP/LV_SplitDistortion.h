/*
  ==============================================================================

    LV_SplitDistortion.h
    Created: 7 Nov 2021 3:30:43pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#ifndef LV_SplitDistortion_h
#define LV_SplitDistortion_h

#include <JuceHeader.h>
#include "LV_SVFilter.h"

class LV_SplitDistortion
{
public:
    
    LV_SplitDistortion();
    
    void prepare(juce::dsp::ProcessSpec& spec);
    
    void processBlock(juce::dsp::AudioBlock<float>& block);
    
    enum class ParameterId
    {
        kPreamp,
        kCurve,
        kWet,
        kTrim,
        kCutoff,
        kBandMode,
        kModelMode,
        kClipInput,
        kBypass
    };
    
    void setParameter(ParameterId parameter, float parameterValue);
    
    enum class BandMode
    {
        kAllBands,
        kLowBand,
        kHighBand
    };
    
    enum class ModelMode
    {
        kInflator,
        kSaturation,
        kTube,
        kSoft,
        kFold,
        kVari,
        kSine
    };
    
private:
    
    void assertRange(float sourceValue, float min, float max) const;
    void assertBool(float sourceValue) const;
    float logScale(float value, float scalar = 20) const;
    void prepareTopBandFilter(juce::dsp::ProcessSpec& spec);
    
    float getInflator(float input);
    float getSaturation(float input);
    float getTubeDistortion(float input);
    float getSoftClip(float input);
    float getFoldClip(float input);
    float getVariClip(float input);
    float getSineClip(float input);
    
    float hardClip(float sample);
    float sm_map(float value, float min, float max, float curve);
    
    float preamp, curve, wet, cutoff, trim, sampleRate;
    bool bypassModule, clipInput;
    
    BandMode bandMode;
    ModelMode modelMode;
    
    juce::dsp::LinkwitzRileyFilter<float> hpFilter;
    juce::dsp::LinkwitzRileyFilter<float> phaseFilter;
    juce::dsp::Gain<float> outputModule;
    
    LV_SVFilter bandOneFilter;
    LV_SVFilter bandTwoFilter;
    
};

#endif /* LV_SplitDistortion_h */
