/*
  ==============================================================================

    FilterMethods.cpp
    Created: 5 Nov 2021 11:38:50pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void ElectronEQAudioProcessor::prepareFilters(juce::dsp::ProcessSpec spec)
{
    highBoostModule.reset();
    highBoostModule.prepare(spec);
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(highBoostDialId) * 1.0);
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(highFreqDialId));
    auto scalar = juce::jmap(float(*treeState.getRawParameterValue(bandwidthDialId)), 0.0f, 10.0f, 0.8f, 0.25f);
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kQ, scalar);
    highBoostModule.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kParametric);
    
    highCutModlue.reset();
    highCutModlue.prepare(spec);
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(highAttenDialId) * -2.0);
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kHighShelf);
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(highFreqDialId) / 4.0);
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kQ, 0.1);

    lowBoostModule.reset();
    lowBoostModule.prepare(spec);
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(lowBoostDialId));
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(lowFreqDialId));
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kQ, 0.05);
    
    lowCutModule.reset();
    lowCutModule.prepare(spec);
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(lowAttenDialId) * -1.0);
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(lowFreqDialId) * 2.0);
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kQ, 0.3);
    
    lpfModule.reset();
    lpfModule.prepare(spec);
    lpfModule.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kLowPass);
    lpfModule.setParameter(LV_SVFilter::ParameterId::kGain, 0.0);
    lpfModule.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(lpfSliderId));
    
    if (*treeState.getRawParameterValue(lpfSliderId) == 20000.0f)
    {
        lpfModule.setParameter(LV_SVFilter::ParameterId::kBypass, true);
    }
    

    highBoostModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    highCutModlue.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    lowBoostModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
    lowCutModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(eqToggleId));
}

void ElectronEQAudioProcessor::prepareTube(juce::dsp::ProcessSpec spec)
{
    tubeModule.reset();
    tubeModule.prepare(spec);
    auto driver = juce::jmap(float(*treeState.getRawParameterValue(driveSliderId)), 0.0f, 24.0f, 0.0f, 6.0f);
    tubeModule.setParameter(LV_TubeModel::ParameterId::kPreamp, driver);
}

void ElectronEQAudioProcessor::prepareTrim(juce::dsp::ProcessSpec spec)
{
    trimModule.reset();
    trimModule.prepare(spec);
    trimModule.setGainDecibels(*treeState.getRawParameterValue(trimSliderId));
}
