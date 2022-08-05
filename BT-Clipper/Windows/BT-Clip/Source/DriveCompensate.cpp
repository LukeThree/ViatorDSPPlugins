/*
  ==============================================================================

    DriveCompensate.cpp
    Created: 14 Oct 2021 7:49:00pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::drive_compensation()
{
    if (m_ClipMenu.getSelectedId() > 1)
    {
        m_OutputSlider.setValue(m_DriveDial.getValue() * -0.375);
    }
    
    else
    {
        m_OutputSlider.setValue(m_DriveDial.getValue() * -0.3125);
    }
}
