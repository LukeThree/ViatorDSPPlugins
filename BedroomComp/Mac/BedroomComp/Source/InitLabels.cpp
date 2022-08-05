/*
  ==============================================================================

    InitLabels.cpp
    Created: 20 Jun 2021 8:53:20pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::initLabels()
{
    for (auto i {0}; i < labels.size(); i++) {
        
        setCommonLabelProps(*labels[i]);
        labels[i]->attachToComponent(dials[i], false);
    }
}
