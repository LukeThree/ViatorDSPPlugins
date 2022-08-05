/*
  ==============================================================================

    InitPostTone.cpp
    Created: 23 Jan 2022 7:02:37pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void BTClipperAudioProcessor::initPostTone()
{
    postToneModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kBandShelf);
    postToneModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(postFreqID));
    postToneModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(postGainID));
    auto newQ = juce::jmap(static_cast<float>(*treeState.getRawParameterValue(postQID)), 0.0f, 100.0f, 0.95f, 0.05f);
    postToneModule.setParameter(LV_SVFilter::ParameterId::kQ, newQ);
    postToneModule.setParameter(LV_SVFilter::ParameterId::kBypass, *treeState.getRawParameterValue(postPowerID));
    postToneModule.setParameter(LV_SVFilter::ParameterId::kClipOutput, true);
}
