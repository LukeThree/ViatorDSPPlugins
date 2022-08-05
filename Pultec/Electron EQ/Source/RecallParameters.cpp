/*
  ==============================================================================

    RecallParameters.cpp
    Created: 5 Nov 2021 11:55:16pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void ElectronEQAudioProcessor::recallFilters()
{
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(highBoostDialId) * 2.0);
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(highFreqDialId));
    
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(highAttenDialId) * -2.0);
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(highFreqDialId) / 4.0);

    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(lowBoostDialId) * 1.5);
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(lowFreqDialId) / 2.0);
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(lowAttenDialId) * -0.5);
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(lowFreqDialId) * 10.0);
    
    lpfModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kLowPass);
    lpfModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(lpfSliderId));
    
    if (*treeState.getRawParameterValue(lpfSliderId) == 20000)
    {
        lpfModule.setParameter(LV_SVFilter::ParameterId::kBypass, true);
    }
}

void ElectronEQAudioProcessor::recallTube()
{
    auto driver = juce::jmap(float(*treeState.getRawParameterValue(driveSliderId)), 0.0f, 24.0f, 0.0f, 6.0f);
    tubeModule.setParameter(LV_TubeModel::ParameterId::kPreamp, driver);
}

void ElectronEQAudioProcessor::recallTrim()
{
    trimModule.setGainDecibels(*treeState.getRawParameterValue(trimSliderId));
}

void ElectronEQAudioProcessor::recallWindow()
{
    windowWidth = variableTree.getProperty("width");
    windowHeight = variableTree.getProperty("height");
}

void ElectronEQAudioProcessor::recallStereoChoice()
{
    stereoMenuChoice = *treeState.getRawParameterValue(stereoMenuId);
}

void ElectronEQAudioProcessor::recallEqBypass()
{
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    lpfModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
}
