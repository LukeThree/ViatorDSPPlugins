/*
  ==============================================================================

    ProcessorCore.cpp
    Created: 14 May 2021 9:27:35pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void LV50AAudioProcessor::callProcessorCore(juce::dsp::AudioBlock<float> block, const int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float* data = block.getChannelPointer(channel);
            
            const float x = data[sample];
            
            const float drive = pow(10.0f, mRawDrive / 20.0f);
            
            //const float softClip = std::atanf(x * drive);
            const float circleMap = x + (drive / juce::MathConstants<float>::twoPi) * std::sin(juce::MathConstants<float>::twoPi * x);
                
            auto mix = (x * (1.0 - 0.5f) + (circleMap * 0.5f));
            
            mix *= 1.1f;
            
            mix *= std::powf(10.0f, -drive * 0.08);
            
            mix *= currentPhase;
            
            data[sample] = mix;
        }
    }
}
