/*
  ==============================================================================

    LV_TubeModel.h
    Created: 13 Jul 2021 11:40:01pm
    Author:  Landon Viator

  ==============================================================================
*/


#pragma once

#ifndef LV_TubeModel_h
#define LV_TubeModel_h

#include <JuceHeader.h>
#include "LV_SVFilter.h"

class LV_TubeModel
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    float processSample(float input, int ch);
    
    enum class ParameterId
    {
        kPreamp,
        kDrive,
        kTrim,
        kBlend,
        kSampleRate
    };

    
    void setParameter(ParameterId parameter, float parameterValue);
    
    float getPreamp();
    
    float getDrive();
    
    float getTrim();
    
    float getBlend();
    
    float getSampleRate();
    
    bool getPowerState();
    
private:
    
    float preamp {0.0};
    float drive {0.0};
    float trim {0.0};
    float blend {1.0};
    float output {0.0};
    
    float gridConductionThreshold {1.5};
    float waveSaturation {0.0};
    float dcOffset {0.0};
    float dcOffsetDetected {0.0};
    float dcShiftCoefficient {1.0};
    float clipPointPositive {4.0};
    float clipPointNegative {-1.5};
    float currentSampleRate {44100.0};
    
    bool powerState = true;
    
    LV_SVFilter dcFilter, midShelf, dcBlockingFilter, upperLPF;
};

#endif /* LV_TubeModel_h */
