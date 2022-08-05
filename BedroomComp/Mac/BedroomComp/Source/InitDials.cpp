/*
  ==============================================================================

    InputDials.cpp
    Created: 20 Jun 2021 4:05:37pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::initDials()
{
    for (auto i {0}; i < dials.size(); i++)
    {
        setCommonDialProps(*dials[i]);
    }
    
    DBG("Input Dials Initialized!");
}
