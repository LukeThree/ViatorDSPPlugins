/*
  ==============================================================================

    InitLabels.cpp
    Created: 21 Aug 2021 1:35:46am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::initLabels()
{
    for (auto i {0}; i < dialLabels.size(); i++)
    {
        addAndMakeVisible(dialLabels[i]);
        dialLabels[i]->attachToComponent(sliders[i], false);
        dialLabels[i]->setText(dialLabelTexts[i], juce::dontSendNotification);
    }
}
