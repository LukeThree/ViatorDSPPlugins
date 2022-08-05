/*
  ==============================================================================

    ProcessorCore.cpp
    Created: 14 May 2021 9:27:35pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void GraphicEQAudioProcessor::callProcessorCore(juce::AudioBuffer<float>& buffer, const int numChannels)
{
    // Main DSP loop
    for (int channel = 0; channel < numChannels; ++channel){
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            
            auto channelBuffers = buffer.getArrayOfWritePointers();
            
            float* data = channelBuffers[channel];
            
            float x = data[sample] * currentPhase;
            
            float output  = tanh((-2.0f * rawDrive) * x + (x * 5.0f)) - tanh(powf(x, 3.0f));
            
            if (rawDrive != 4.0f){
                
                data[sample] = output * 0.9f * pow(10.0f, -2.0 * rawDrive / 30);

            } else {
                
                data[sample] = x;
            }
        }
    }
    
    // Trim gain and smoothing
        if (currentTrimGain == lastTrimGain)
        {
            buffer.applyGain(0, buffer.getNumSamples(), currentTrimGain);

        } else
        {
            buffer.applyGainRamp(0, buffer.getNumSamples(), lastTrimGain, currentTrimGain);
            lastTrimGain = currentTrimGain;
        }
}
