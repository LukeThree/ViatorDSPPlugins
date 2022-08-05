/*
  ==============================================================================

    ButtonProps.cpp
    Created: 20 Jun 2021 7:36:24pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setCommonButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(&button);
    button.setClickingTogglesState(true);
    button.setColour(0x1000100, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));
    button.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    button.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    button.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    button.setColour(0x1000103, juce::Colours::black.brighter(0.1));
}
