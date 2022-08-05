/*
  ==============================================================================

    EQButtons.cpp
    Created: 21 Jun 2021 12:35:38am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setEQButtonProps()
{
    eqToggle.setButtonText("In");
    lowPeakToggle.setButtonText("Peak");
    midQToggle.setButtonText("Mid Hi Q");
    highPeakToggle.setButtonText("Peak");
    
    highFreqToggle.setButtonText("8K");

    highFreqToggle.setColour(0x1000101, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));

    highFreqToggle.onClick = [&]()
    {
        const auto message = highFreqToggle.getToggleState() ? "16K" : "8K";
        
        highFreqToggle.setButtonText(message);
    };
}
