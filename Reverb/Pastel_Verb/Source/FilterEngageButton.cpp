/*
  ==============================================================================

    FilterEngageButton.cpp
    Created: 21 Aug 2021 2:12:18am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::setSetFilterEngageButtonProps(juce::TextButton &button)
{
    // Individual button props
    button.setButtonText("In");
        
    // Parameter attachment
    filterEngageButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, filterEngageId, filterEngageButton);

    // Change the button text based on the state
    const auto message = button.getToggleState() ? "In" : "Out";
    button.setButtonText(message);
    button.onClick = [&]()
    {
        const auto message = button.getToggleState() ? "In" : "Out";
        button.setButtonText(message);
        filterModeButton.setEnabled(button.getToggleState());
        cutoffSlider.setEnabled(button.getToggleState());
        resonanceSlider.setEnabled(button.getToggleState());
        driveSlider.setEnabled(button.getToggleState());
    };
}
