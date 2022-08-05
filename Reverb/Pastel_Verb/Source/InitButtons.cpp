/*
  ==============================================================================

    InitButtons.cpp
    Created: 21 Aug 2021 1:14:19am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::initButtons()
{
    addAndMakeVisible(filterModeButton);
    addAndMakeVisible(filterEngageButton);
    setSetFilterEngageButtonProps(filterEngageButton);
    setSetFilterModeButtonProps(filterModeButton);
}
