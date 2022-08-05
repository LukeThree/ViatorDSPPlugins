/*
  ==============================================================================

    InitDSPVariables.cpp
    Created: 30 Nov 2021 6:42:12pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void PhonemodelAudioProcessor::initDSPVariables()
{
    // Initialize dsp variable states
    initCutoffMultipliers(static_cast<float>(*treeState.getRawParameterValue(bandwidthID)));
    initCutoffs(static_cast<float>(*treeState.getRawParameterValue(centerID)));
    drive = juce::jmap(static_cast<float>(*treeState.getRawParameterValue(driveID)), 0.0f, 12.0f, 1.0f, -0.1f);
    qWidth = juce::jmap(static_cast<float>(*treeState.getRawParameterValue(resoID)), 0.0f, 100.0f, 0.5f, 9.0f);
    midGain = juce::jmap(static_cast<float>(*treeState.getRawParameterValue(resoID)), 0.0f, 100.0f, 0.0f, 12.0f);
    drive = juce::jmap(static_cast<float>(*treeState.getRawParameterValue(driveID)), 0.0f, 12.0f, 1.0f, 0.45f);
    inputRectifierModule.setParameter(LV_Rectifier::ParameterId::kDrive, drive);
    outputRectifierModule.setParameter(LV_Rectifier::ParameterId::kDrive, drive);
}

void PhonemodelAudioProcessor::initFilters()
{
    updateLowFilter();
    updateHighFilter();
    updateMidToneFilter(centerFreq, qWidth, midGain);
    updateLowCompensateFilter();
    updateLowShelfFilter();
    updateHighShelfFilter();
}
