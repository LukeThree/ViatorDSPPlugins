/*
  ==============================================================================

    InitPreTone.cpp
    Created: 22 Jan 2022 7:40:58pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void JustaFilterAudioProcessor::initPreTone()
{
    preToneModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kBandShelf);
    preToneModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(preFreqID));
    preToneModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(preGainID));
    auto newQ = juce::jmap(static_cast<float>(*treeState.getRawParameterValue(preQID)), 0.0f, 100.0f, 0.95f, 0.05f);
    preToneModule.setParameter(LV_SVFilter::ParameterId::kQ, newQ);
    preToneModule.setParameter(LV_SVFilter::ParameterId::kBypass, *treeState.getRawParameterValue(prePowerID));
}
