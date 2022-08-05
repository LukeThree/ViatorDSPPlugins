/*
  ==============================================================================

    Parameters.cpp
    Created: 25 Jun 2021 9:24:47pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "Parameters.h"
#include "PluginProcessor.h"
#include "LV_Compressor.h"

juce::AudioProcessorValueTreeState::ParameterLayout BedroomCompAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Make sure to update the number of reservations after adding params
    params.reserve(8);
    
    auto pThresh = std::make_unique<juce::AudioParameterFloat>(threshId, threshName, -25.0, 20.0, 8.0);
    auto pRatio = std::make_unique<juce::AudioParameterFloat>(ratioId, ratioName, 0.0, 5.0, 0.0);
    auto pSpeed = std::make_unique<juce::AudioParameterFloat>(speedId, speedName, 0.0, 5.0, 0.0);
    auto pBlend = std::make_unique<juce::AudioParameterFloat>(blendId, blendName, 0.0, 100.0, 0.0);
    auto pMakeUp = std::make_unique<juce::AudioParameterFloat>(makeUpId, makeUpName, -6.0, 20.0, 0.0);
    auto pCompIn = std::make_unique<juce::AudioParameterBool>(compressorEnableId, compressorEnableName, false);
    auto pHpf = std::make_unique<juce::AudioParameterFloat>(hpfId, hpfId, 20.0, 350.0, 0.0);
    auto pHpfEnable = std::make_unique<juce::AudioParameterBool>(hpfEnabledId, hpfEnabledName, false);

    params.push_back(std::move(pThresh));
    params.push_back(std::move(pRatio));
    params.push_back(std::move(pSpeed));
    params.push_back(std::move(pBlend));
    params.push_back(std::move(pMakeUp));
    params.push_back(std::move(pCompIn));
    params.push_back(std::move(pHpf));
    params.push_back(std::move(pHpfEnable));

    return { params.begin(), params.end() };
}

void BedroomCompAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == threshId)
    {
        compressorModule.setParameter(LV_Compressor::ParameterId::kThresh, newValue);
    }
    
    if (parameterID == ratioId)
    {
        compressorModule.setParameter(LV_Compressor::ParameterId::kRatio, newValue);
    }
    
    if (parameterID == speedId)
    {
        compressorModule.setParameter(LV_Compressor::ParameterId::kSpeedValueStringsVec, *treeState.getRawParameterValue(speedId));
        compressorModule.setParameter(LV_Compressor::ParameterId::kTwoDSpeedValues, *treeState.getRawParameterValue(speedId));

    }
    
    if (parameterID == blendId)
    {
        compressorModule.setParameter(LV_Compressor::ParameterId::kBlend, newValue);
    }
    
    if (parameterID == makeUpId)
    {
        compressorModule.setParameter(LV_Compressor::ParameterId::kMakeUp, newValue);
    }
    
    if (parameterID == compressorEnableId)
    {
        compressorModule.setParameter(LV_Compressor::ParameterId::kEnabled, newValue);
    }
    
    if (parameterID == hpfId)
    {
        for (auto i {0}; i < hpFilters.size(); i++)
        {
            hpFilters[i]->setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
        }
    }
    
    if (parameterID == hpfEnabledId)
    {
        //compressorFilterModule.setParameter(LV_SVFilter::ParameterId::kBypass, *treeState.getRawParameterValue(hpfEnabledId));

        //inputHPFilterModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(hpfEnabledId));
    }
}

float BedroomCompAudioProcessor::getRatioForDial()
{
    return  compressorModule.getRatio();
}

std::string BedroomCompAudioProcessor::getSpeedDialStrings()
{
    return compressorModule.getSpeedValueString();
}
