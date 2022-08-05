/*
  ==============================================================================

    DialProps.cpp
    Created: 10 Oct 2021 1:17:47pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::set_dial_props()
{
    m_CutoffDial.setRange(300.0, 5000.0, 1.0);
    m_CutoffDial.setSkewFactorFromMidPoint(1000.0);
}
