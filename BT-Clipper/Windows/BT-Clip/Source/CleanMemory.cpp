/*
  ==============================================================================

    CleanMemory.cpp
    Created: 9 Oct 2021 2:17:57am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::clean_memory()
{
    for (auto i {0}; i < sliders.size(); i++)
    {
        sliders[i]->setLookAndFeel(nullptr);
    }
    
    m_InputSlider.setLookAndFeel(nullptr);
    m_OutputSlider.setLookAndFeel(nullptr);
}
