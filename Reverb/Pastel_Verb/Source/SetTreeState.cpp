/*
  ==============================================================================

    SetTreeState.cpp
    Created: 21 Aug 2021 3:55:13pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void Pastel_VerbAudioProcessor::setStateForTreeParameters()
{
    windowWidth = variableTree.getProperty("width");
    windowHeight = variableTree.getProperty("height");
    
    ladderProcessor.setCutoffFrequencyHz(*treeState.getRawParameterValue(cutoffSliderId));
    ladderProcessor.setResonance(*treeState.getRawParameterValue(resonanceSliderId) * 0.01);
    ladderProcessor.setDrive(pow(10, *treeState.getRawParameterValue(driveSliderId) / 20));
    
    if (*treeState.getRawParameterValue(filterEngageId) == 0)
    {
        ladderProcessor.setEnabled(false);
    }
    
    else
    {
        ladderProcessor.setEnabled(true);
    }
    
    if (*treeState.getRawParameterValue(filterModeId) == 0)
    {
        ladderProcessor.setMode(juce::dsp::LadderFilterMode::LPF12);
    }
    else
    {
        ladderProcessor.setMode(juce::dsp::LadderFilterMode::LPF24);
    }
    
    reverbParams.roomSize = *treeState.getRawParameterValue(roomSizeSliderId) * .01;
    reverbParams.damping = *treeState.getRawParameterValue(dampingSliderId) * .01;
    reverbParams.width = *treeState.getRawParameterValue(widthSliderId) * .01;
    reverbParams.dryLevel = *treeState.getRawParameterValue(drySliderId) * .01;
    reverbParams.wetLevel = *treeState.getRawParameterValue(wetSliderId) * .01;
    reverbProcessor.setParameters(reverbParams);
}
