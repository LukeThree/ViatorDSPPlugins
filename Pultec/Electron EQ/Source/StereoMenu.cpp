/*
  ==============================================================================

    StereoMenu.cpp
    Created: 12 Jul 2021 11:51:03pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void ElectronEQAudioProcessorEditor::setStereoBoxProps(juce::ComboBox &menu)
{
    menu.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black.withAlpha(0.35f));
    menu.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    menu.setColour(juce::ComboBox::outlineColourId, juce::Colours::whitesmoke.withAlpha(0.0f));
    menu.setColour(juce::ComboBox::arrowColourId, juce::Colours::whitesmoke.darker(1.0f));
    menu.getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black.brighter(0.12));
    menu.getLookAndFeel().setColour(juce::PopupMenu::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    menu.getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::whitesmoke.withAlpha(0.05f));
    menu.setText("Stereo Mode");
    menu.addItem("Stereo", 1);
    menu.addItem("Sides", 2);
    menu.addItem("Mid", 3);
    menu.addItem("Left", 4);
    menu.addItem("Right", 5);
    stereoMenuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, stereoMenuId, stereoMenu);
    addAndMakeVisible(menu);
}
