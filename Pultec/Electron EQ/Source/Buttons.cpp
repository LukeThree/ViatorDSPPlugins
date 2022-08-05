/*
  ==============================================================================

    Buttons.cpp
    Created: 13 Jul 2021 12:08:15am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void ElectronEQAudioProcessorEditor::initButtons()
{
    for (auto i {0}; i < buttons.size(); i++)
    {
        setCommonButtonProps(*buttons[i]);
    }
}

void ElectronEQAudioProcessorEditor::setCommonButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(button);
    button.setClickingTogglesState(true);
    button.setColour(0x1000100, juce::Colours::black.withAlpha(0.35f));
    button.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    button.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    button.setColour(0x1000102, juce::Colours::whitesmoke.withAlpha(0.25f));
    button.setColour(0x1000103, juce::Colours::black.brighter(0.1));
    
    oversampleToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, osToggleId, oversampleToggle);
    oversampleToggle.setButtonText("OS");
    
    enabledToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, eqToggleId, enabledToggle);

    const auto message = enabledToggle.getToggleState() ? "EQ In" : "EQ Out";

    enabledToggle.setButtonText(message);

    enabledToggle.onClick = [&]()
    {
        const auto message = enabledToggle.getToggleState() ? "EQ In" : "EQ Out";

        enabledToggle.setButtonText(message);
    };
}
