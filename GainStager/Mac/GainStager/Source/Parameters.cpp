/*
  ==============================================================================

    Parameters.cpp
    Created: 10 Jun 2021 11:32:55pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "Parameters.h"
#include "PluginProcessor.h"

juce::AudioProcessorValueTreeState::ParameterLayout GainStagerAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    // Make sure to update the number of reservations after adding params
    params.reserve(5);

    auto pPreamp = std::make_unique<juce::AudioParameterFloat>(preampSliderId, preampSliderName, -100.0f, 100.0f, 0.0f);
    auto pPhase = std::make_unique<juce::AudioParameterBool>(phaseId, phaseName, false);
    auto pReset = std::make_unique<juce::AudioParameterBool>(listenId, listenName, false);
    auto pStageType = std::make_unique<juce::AudioParameterInt>(stageTypeId, stageTypeName, 0, 1, 0);
    auto pCalibration = std::make_unique<juce::AudioParameterInt>(calibrationId, calibrationName, 0, 3, 0);

    params.push_back(std::move(pPreamp));
    params.push_back(std::move(pPhase));
    params.push_back(std::move(pReset));
    params.push_back(std::move(pStageType));
    params.push_back(std::move(pCalibration));

   return { params.begin(), params.end() };
}

void GainStagerAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue){
    
    // Update params here
    if (parameterID == preampSliderId)
    {
        preampProcessor.setGainDecibels(newValue);
    }

    else  if (parameterID == phaseId)
    {
        setPhase(newValue);
    }

    else if (parameterID == listenId)
    {
        if (newValue > -1)
        {
            avgRMS = 0.0f;
            
            avgVector.clear();

            treeState.getParameterAsValue(preampSliderId) = 0.0f;
            
            averageHasRun = false;

            loopCount = 0;
            
            listen = true;

            DBG("Listen");
        }
    }
    
    else if (parameterID == stageTypeId)
    {
        stageType = newValue;

        switch (int(newValue)) {

            case 0: treeState.getParameterAsValue(preampSliderId) = thresh - float(variableTree.getProperty("temp average")) - 9.0f; break;
            case 1: treeState.getParameterAsValue(preampSliderId) = thresh - float(variableTree.getProperty("temp peak")); break;
        }
    }

    else if (parameterID == calibrationId)
    {
        switch (int(newValue)) {

            case 0: thresh = -18.0f; break;
            case 1: thresh = -21.0f; break;
            case 2: thresh = -15.0f; break;
            case 3: thresh = -12.0f; break;

        }
    }
}
