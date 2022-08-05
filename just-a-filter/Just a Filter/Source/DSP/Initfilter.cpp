/*
  ==============================================================================

    Initfilter.cpp
    Created: 3 Feb 2022 8:18:57pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void JustaFilterAudioProcessor::initFilter()
{
    filterModule.setParameter(LV_SVFilter::ParameterId::kType, *treeState.getRawParameterValue(filterTypeID));
    filterModule.setParameter(LV_SVFilter::ParameterId::kQType, *treeState.getRawParameterValue(bandwidthTypeID));
    filterModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(cutoffID));
    filterModule.setParameter(LV_SVFilter::ParameterId::kQ, *treeState.getRawParameterValue(bandwidthID));
    filterModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(gainID));
    trimModule.setGainDecibels(*treeState.getRawParameterValue(trimID));
}
