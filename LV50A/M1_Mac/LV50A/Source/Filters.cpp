/*
  ==============================================================================

    Filters.cpp
    Created: 26 May 2021 8:11:22pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void LV50AAudioProcessor::prepareEQFilters(juce::dsp::ProcessSpec spec)
{
    // EQ Filters
    mLSFilter.prepare(spec);
    mLSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(freq1Id));
    mLSFilter.setParameter(LV_SVFilter::ParameterId::kQ, 0.05f);
    mLSFilter.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(gain1Id));
    mLSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kLowShelf);
    mLSFilter.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kProportional);
    
    mBPFilter1.prepare(spec);
    mBPFilter1.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(freq2Id));
    mBPFilter1.setParameter(LV_SVFilter::ParameterId::kQ, 0.05f);
    mBPFilter1.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(gain2Id));
    mBPFilter1.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kBandShelf);
    mBPFilter1.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kProportional);
    
    mBPFilter2.prepare(spec);
    mBPFilter2.setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(freq3Id));
    mBPFilter2.setParameter(LV_SVFilter::ParameterId::kQ, 0.05f);
    mBPFilter2.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(gain3Id));
    mBPFilter2.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kBandShelf);
    mBPFilter2.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kProportional);
    
    mHSFilter.prepare(spec);
    mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(*treeState.getRawParameterValue(freq4Id)) * 0.6f);
    mHSFilter.setParameter(LV_SVFilter::ParameterId::kQ, 0.05f);
    mHSFilter.setParameter(LV_SVFilter::ParameterId::kGain, *treeState.getRawParameterValue(gain4Id));
    mHSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kHighShelf);
    mHSFilter.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kProportional);
}

void LV50AAudioProcessor::prepareFilters(juce::dsp::ProcessSpec spec)
{
    // Filters
    mLPFilter.prepare(spec);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, 20000.0f);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kQ, 0.05f);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kGain, 0);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kLowPass);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kParametric);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(filtersBId));
    
    mHPFilter.prepare(spec);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, 40.0f);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kQ, 0.05f);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kGain, 0);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kHighPass);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kQType, LV_SVFilter::QType::kParametric);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(filtersBId));
    
    // Initialize filter type based on what's saved in the ValueTree
    if (*treeState.getRawParameterValue(highShelfBId) == 0) {
        
        mHSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
        mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(*treeState.getRawParameterValue(freq4Id)));
        
    } else {
        
        mHSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kHighShelf);
        mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(*treeState.getRawParameterValue(freq4Id)) * 0.6f);
    }
    
    if (*treeState.getRawParameterValue(lowShelfBId) == 0) {
        
        mLSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
        
    } else {
        
        mLSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kLowShelf);
    }
}

void LV50AAudioProcessor::setAllSampleRates(float value)
{
    mLSFilter.setParameter(LV_SVFilter::ParameterId::kSampleRate, value);
    mBPFilter1.setParameter(LV_SVFilter::ParameterId::kSampleRate, value);
    mBPFilter2.setParameter(LV_SVFilter::ParameterId::kSampleRate, value);
    mHSFilter.setParameter(LV_SVFilter::ParameterId::kSampleRate, value);
    mHPFilter.setParameter(LV_SVFilter::ParameterId::kSampleRate, value);
    mLPFilter.setParameter(LV_SVFilter::ParameterId::kSampleRate, value);
}

float LV50AAudioProcessor::proportionalShelfFilterQ(float gain, bool lowShelf)
{
    if (lowShelf) {
        
        return powf(10.0f, gain / 55.0f) - 0.95f;
        
    } else {
        
        return powf(10.0f, gain / 65.0f) - 0.95f;
    }
}

float LV50AAudioProcessor::proportionalPeakFilterQ(float gain)
{
    if (gain >= 0.0f) {
        
        return powf(10.0f, gain / 65.0f) - 0.95f;
        
    } else {
        
        return powf(10.0f, gain / 30.0f) - 0.29f;
    }
}
