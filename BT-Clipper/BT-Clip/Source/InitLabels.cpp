/*
  ==============================================================================

    InitLabels.cpp
    Created: 9 Oct 2021 2:12:43am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::init_labels()
{
    for (auto i {0}; i < labels.size(); i++)
    {
        labels[i]->attachToComponent(sliders[i], false);
    }
    
    m_InputLabel.attachToComponent(&m_InputSlider, false);
    m_OutputLabel.attachToComponent(&m_OutputSlider, false);
}
