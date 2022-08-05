/*
  ==============================================================================

    AddRemoveParams.cpp
    Created: 13 Oct 2021 7:20:39pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void BTClipAudioProcessor::add_params()
{
    treeState.addParameterListener (inputSliderId, this);
    treeState.addParameterListener (outputSliderId, this);
    treeState.addParameterListener (cutoffSliderId, this);
    treeState.addParameterListener (midGainSliderId, this);
    treeState.addParameterListener (driveSliderId, this);
    treeState.addParameterListener (filterToggleId, this);
    treeState.addParameterListener (phaseId, this);
    treeState.addParameterListener (driveModelId, this);
}

void BTClipAudioProcessor::remove_params()
{
    treeState.removeParameterListener (inputSliderId, this);
    treeState.removeParameterListener (outputSliderId, this);
    treeState.removeParameterListener (cutoffSliderId, this);
    treeState.removeParameterListener (midGainSliderId, this);
    treeState.removeParameterListener (driveSliderId, this);
    treeState.removeParameterListener (filterToggleId, this);
    treeState.removeParameterListener (phaseId, this);
    treeState.removeParameterListener (driveModelId, this);
}
