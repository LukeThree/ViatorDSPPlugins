/*
  ==============================================================================

    Labels.cpp
    Created: 13 Jul 2021 6:02:48pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void ElectronEQAudioProcessorEditor::initLabels()
{
    for (auto i{0}; i < dialLabels.size(); i++)
    {
        setCommonLabelProps(*dialLabels[i]);
        dialLabels[i]->attachToComponent(dials[i], false);
        dialLabels[i]->setText(dialLabelTexts[i], juce::dontSendNotification);
    }
    
    for (auto i{0}; i < sliderLabels.size(); i++)
    {
        setCommonLabelProps(*sliderLabels[i]);
        sliderLabels[i]->attachToComponent(sliders[i], false);
        sliderLabels[i]->setText(sliderLabelTexts[i], juce::dontSendNotification);
    }
}

void ElectronEQAudioProcessorEditor::setCommonLabelProps(juce::Label &label)
{
    addAndMakeVisible(&label);
    label.setJustificationType(juce::Justification::centredTop);
    label.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    label.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    label.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
}
