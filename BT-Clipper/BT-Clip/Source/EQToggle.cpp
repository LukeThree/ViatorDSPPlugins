/*
  ==============================================================================

    EQToggle.cpp
    Created: 10 Oct 2021 3:18:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::set_pre_post_toggle_props()
{
    const auto message = m_PrePostToggle.getToggleState() ? "Post" : "Pre";
    
    m_PrePostToggle.setButtonText(message);
    
    m_PrePostToggle.onClick = [&]()
    {
        const auto message = m_PrePostToggle.getToggleState() ? "Post" : "Pre";
        
        m_PrePostToggle.setButtonText(message);
    };
}
