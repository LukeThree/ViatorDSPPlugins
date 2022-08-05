/*
  ==============================================================================

    MainDSPLoop.cpp
    Created: 13 Oct 2021 7:42:26pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void BTClipAudioProcessor::do_dsp_loop(juce::AudioBuffer<float> &buffer)
{
    // Main DSP Loop
    auto channelBuffers = buffer.getArrayOfWritePointers();

    for (auto sample {0}; sample < buffer.getNumSamples(); sample++)
    {
        for (auto channel {0}; channel < buffer.getNumChannels(); channel++)
        {
            auto x = channelBuffers[channel][sample];
            
            x *= m_Preamp;

            if (m_PostEq)
            {
                x = m_MidToneModule.processSample(x, channel);
                x = m_ClippersModule.processSample(x);
            }
            
            else
            {
                x = m_ClippersModule.processSample(x);
                x = m_MidToneModule.processSample(x, channel);
            }
            
            x *= m_Trim;
            
            // Avoiding if statement with genius math
            x *= m_Phase * 2.0 - 1.0;
            
            channelBuffers[channel][sample] = x;
        }
    }
}
