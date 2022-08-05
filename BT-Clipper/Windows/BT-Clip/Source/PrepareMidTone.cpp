/*
  ==============================================================================

    PrepareMidTone.cpp
    Created: 13 Oct 2021 7:33:52pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void BTClipAudioProcessor::prepare_mid_tone(juce::dsp::ProcessSpec spec)
{
    m_MidToneModule.reset();
    m_MidToneModule.prepare(spec);
    m_MidToneModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kBandShelf);
    m_MidToneModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(cutoffSliderId));
    m_MidToneModule.setParameter(LV_SVFilter::ParameterId::kQ, 0.33);
    m_MidToneModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(midGainSliderId));
}
