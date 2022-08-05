/*
  ==============================================================================

    CreateParams.cpp
    Created: 13 Oct 2021 7:23:32pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

juce::AudioProcessorValueTreeState::ParameterLayout BTClipAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    // Make sure to update the number of reservations after adding params
    params.reserve(8);

    auto p_FilterToggle = std::make_unique<juce::AudioParameterBool>(filterToggleId, filterToggleName, false);
    auto p_Phase = std::make_unique<juce::AudioParameterBool>(phaseId, phaseName, false);
    auto p_DriveModel = std::make_unique<juce::AudioParameterInt>(driveModelId, driveModelName, 0, 2, 0);
    auto p_Input = std::make_unique<juce::AudioParameterFloat>(inputSliderId, inputSliderName, -40.0, 40.0, 0.0);
    auto p_Output = std::make_unique<juce::AudioParameterFloat>(outputSliderId, outputSliderName, -40.0, 40.0, 0.0);
    auto p_Cutoff = std::make_unique<juce::AudioParameterFloat>(cutoffSliderId, cutoffSliderName, 300, 5000, 1000);
    auto p_MidGain = std::make_unique<juce::AudioParameterFloat>(midGainSliderId, midGainSliderName, -10.0, 10.0, 0.0);
    auto p_Drive = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0, 32.0, 0.0);
    
    params.push_back(std::move(p_FilterToggle));
    params.push_back(std::move(p_Phase));
    params.push_back(std::move(p_DriveModel));
    params.push_back(std::move(p_Input));
    params.push_back(std::move(p_Output));
    params.push_back(std::move(p_Cutoff));
    params.push_back(std::move(p_MidGain));
    params.push_back(std::move(p_Drive));

   return { params.begin(), params.end() };
}
