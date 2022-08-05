/*
  ==============================================================================

    PedalDistortion.cpp
    Created: 25 May 2021 3:19:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PedalDistortion.h"

void PedalDistortion::reset()
{
    
}

void PedalDistortion::prepare(juce::dsp::ProcessSpec spec)
{
    mCurrentSampleRate = spec.sampleRate;
}

void PedalDistortion::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter) {
            
        case ParameterId::kPreamp: mCurrentPreampGain = parameterValue; break;
        case ParameterId::kDrive: mDrive = parameterValue; break;
        case ParameterId::kTrim: mCurrentTrimGain = parameterValue; break;
    }
}

float PedalDistortion::convertGain(const float value)
{
    return pow(10.0f, value / 20.0f);
}

void PedalDistortion::process(juce::AudioBuffer<float>& buffer, const int numChannels)
{
    auto channelBuffers = buffer.getArrayOfWritePointers();
    
    // Main DSP loop
    for (int channel = 0; channel < numChannels; ++channel){
        
        float* data = channelBuffers[channel];

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            
            auto x = data[sample];
                
            data[sample] = tanh(-mDrive * x + x) - tanh(pow(x, 3.0f)) * 0.55f;
        }
    }
}
