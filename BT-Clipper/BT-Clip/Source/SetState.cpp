/*
  ==============================================================================

    SetState.cpp
    Created: 13 Oct 2021 7:44:26pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void BTClipAudioProcessor::set_state()
{
    m_WindowWidth = variableTree.getProperty("width");
    m_WindowHeight = variableTree.getProperty("height");
    
    m_PostEq = *treeState.getRawParameterValue(filterToggleId);
    m_Preamp = pow(10.0f, *treeState.getRawParameterValue(inputSliderId) * 0.05f);
    m_Trim = pow(10.0f, *treeState.getRawParameterValue(outputSliderId) * 0.05f);
    m_Phase = *treeState.getRawParameterValue(phaseId);
    
    // Recall drive type
    m_Drive_Type = *treeState.getRawParameterValue(driveModelId);
    
    if (m_Drive_Type == 0)
    {
        m_ClippersModule.set_clipping_type(LV_Clippers::ClippingType::kHardClip);
    }

    else if (m_Drive_Type == 1)
    {
        m_ClippersModule.set_clipping_type(LV_Clippers::ClippingType::kSoftClip);
    }

    else
    {
        m_ClippersModule.set_clipping_type(LV_Clippers::ClippingType::kAnalog);
    }
    
    // Recall drive settings
    m_Drive = *treeState.getRawParameterValue(driveSliderId);
    m_ClippersModule.setParameter(LV_Clippers::ParameterId::kDrive, m_Drive);
}
