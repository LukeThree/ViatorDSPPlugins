/*
  ==============================================================================

    PedalDistortion.h
    Created: 25 May 2021 3:19:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#ifndef PedalDistortion_h
#define PedalDistortion_h

#include <JuceHeader.h>

class PedalDistortion
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    void process(juce::AudioBuffer<float>& buffer, const int numChannels);
    
    enum class ParameterId {
        kPreamp,
        kDrive,
        kTrim,
    };
    
    void setParameter(ParameterId parameter, float parameterValue);
    
    float convertGain(const float value);
    
private:
    
    // Member variables
    float mLastPreampGain, mCurrentPreampGain, mLastTrimGain, mCurrentTrimGain, mDrive, mCurrentSampleRate;
};

#endif /* PedalDistortion_h */
