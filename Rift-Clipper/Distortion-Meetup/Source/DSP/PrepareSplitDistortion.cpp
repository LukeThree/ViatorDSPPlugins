/*
  ==============================================================================

    PrepareSplitDistortion.cpp
    Created: 9 Nov 2021 5:25:38pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void DistortionMeetupAudioProcessor::prepareSplitDistortion(juce::dsp::ProcessSpec spec)
{
    splitDistortionModule.prepare(spec);
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kPreamp, *treeState.getRawParameterValue(inputID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCurve, *treeState.getRawParameterValue(driveID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kWet, *treeState.getRawParameterValue(mixID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kTrim, *treeState.getRawParameterValue(outputID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCutoff, *treeState.getRawParameterValue(crossOverID));
}
