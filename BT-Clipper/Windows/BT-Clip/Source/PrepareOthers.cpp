/*
  ==============================================================================

    PrepareOthers.cpp
    Created: 13 Oct 2021 7:39:32pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void BTClipAudioProcessor::prepare_others()
{
    m_PostEq = *treeState.getRawParameterValue(filterToggleId);
    m_Phase = *treeState.getRawParameterValue(phaseId);
    m_Preamp = pow(10.0f, *treeState.getRawParameterValue(inputSliderId) * 0.05f);
    m_Trim = pow(10.0f, *treeState.getRawParameterValue(outputSliderId) * 0.05f);
}
