/*
  ==============================================================================

    BlockChain.cpp
    Created: 8 May 2021 3:37:38pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void OverdrivePedalAudioProcessor::callPreOSChain(juce::dsp::AudioBlock<float> block)
{
    hpFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OverdrivePedalAudioProcessor::callPostOSChain(juce::dsp::AudioBlock<float> block)
{
    lpFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    hsFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    
}
