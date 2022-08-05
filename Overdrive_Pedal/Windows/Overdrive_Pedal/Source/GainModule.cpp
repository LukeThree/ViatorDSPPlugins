/*
  ==============================================================================

    GainModule.cpp
    Created: 8 May 2021 3:54:51pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void OverdrivePedalAudioProcessor::prepareGains(juce::dsp::ProcessSpec spec)
{
    lastPreampGain = pow(10, *treeState.getRawParameterValue(inputSliderId) / 20);
    currentPreampGain = pow(10, *treeState.getRawParameterValue(inputSliderId) / 20);
    
    lastTrimGain = pow(10, *treeState.getRawParameterValue(trimSliderId) / 20);
    currentTrimGain = pow(10, *treeState.getRawParameterValue(trimSliderId) / 20);
    
}
