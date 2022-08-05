/*
  ==============================================================================

    LV_Compressor.h
    Created: 29 Jun 2021 12:43:26am
    Author:  Landon Viator

  ==============================================================================
*/

#ifndef LV_Compressor_h
#define LV_Compressor_h

#include <JuceHeader.h>
#include "LV_SVFilter.h"

class LV_Compressor
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    float processSample(float input, float hpSample, bool hpSC);
    
    enum class ParameterId {
            kThresh,
            kRatio,
            kAttack,
            kRelease,
            kBlend,
            kSampleRate,
            kMakeUp,
            kEnabled,
            kSpeedValueStringsVec,
            kTwoDSpeedValues
        };

    
    void setParameter(ParameterId parameter, float parameterValue);
    
    float getCurrentSignal();
    
    float getThresh();
    
    float getAtttack();

    float getRelease();

    float getScaledMakeUp();

    float getBlend();

    float getScaledBlend();
    
    float getMakeUp();
    
    float getRatio();
    
    std::string getSpeedValueString();
    
private:
    
    float thresh, ratio {1.5}, attack, release, gainSC, gainSmooth, blend, scaledBlend, makeUp, scaledMakeUp {1.0}, currentSampleRate, currentSignal;
    
    float gainSmoothPrevious = 0;
    float gainChange_dB = 0;
    
    bool enabled = true;
    
    std::string speedDialString;

    std::vector<float> ratioValues = {
        
        1.5, 2.0, 3.0, 4.0, 6.0, 8.0
    };
    
    std::vector<std::string> speedValueStrings {
        
        "Fast", "MF", "Med", "MS", "Slow", "Auto"
    };
    
    std::vector<std::vector<float>> twoDSpeedValues
    {
        {1.8, 150.0},
        {5.25, 250.0},
        {18.0, 350.0},
        {38.0, 600.0},
        {72.5, 800.0},
        {10.0, 400.0},
    };
};

#endif /* LV_Compressor_h */
