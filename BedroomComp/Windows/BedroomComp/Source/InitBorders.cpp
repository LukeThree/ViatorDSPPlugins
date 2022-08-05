/*
  ==============================================================================

    InitBorders.cpp
    Created: 22 Jun 2021 11:20:43pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::initBorders()
{
    for (auto i {0}; i < borders.size(); i++)
    {
        setBorderProps(*borders[i]);
    }
}
