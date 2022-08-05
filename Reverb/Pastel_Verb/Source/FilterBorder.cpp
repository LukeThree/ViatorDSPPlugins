/*
  ==============================================================================

    FilterBorder.cpp
    Created: 21 Aug 2021 2:51:51pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::initFilterBorder()
{
    addAndMakeVisible(filterBorder);
    filterBorder.setText("Filter");
}
