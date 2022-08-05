/*
  ==============================================================================

    SatTypeButton.cpp
    Created: 20 Jun 2021 7:57:28pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setSaturationTypeToggleProps()
{
    saturationTypeToggle.setButtonText("Red");

    saturationTypeToggle.setColour(0x1000101, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));

    saturationTypeToggle.onClick = [&]()
    {
        const auto message = saturationTypeToggle.getToggleState() ? "Blue" : "Red";
        
        saturationTypeToggle.setButtonText(message);
    };
}
