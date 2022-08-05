/*
  ==============================================================================

    InitButtons.cpp
    Created: 20 Jun 2021 7:40:44pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::initButtons()
{
    for (auto i {0}; i < buttons.size(); i++)
    {
        setCommonButtonProps(*buttons[i]);
    }
    
    setEQButtonProps();
    setCompButtonProps();
    
    DBG("Buttons initialized!");
}
