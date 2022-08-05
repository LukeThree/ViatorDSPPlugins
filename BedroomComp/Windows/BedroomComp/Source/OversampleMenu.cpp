/*
  ==============================================================================

    OversampleMenu.cpp
    Created: 20 Jun 2021 4:42:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setOversampleMenuProps()
{
    // Sample menu
    addAndMakeVisible(mSampleMenu);
    mSampleMenu.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black.brighter(0.25).withAlpha(0.0f));
    mSampleMenu.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    mSampleMenu.setColour(juce::ComboBox::outlineColourId, juce::Colours::whitesmoke.withAlpha(0.0f));
    mSampleMenu.setColour(juce::ComboBox::arrowColourId, juce::Colours::whitesmoke.darker(1.0f));
    mSampleMenu.getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black.brighter(0.12));
    mSampleMenu.getLookAndFeel().setColour(juce::PopupMenu::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    mSampleMenu.getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::whitesmoke.withAlpha(0.05f));
    mSampleMenu.setText("Oversampling");
    mSampleMenu.addItem("Oversample Off", 1);
    mSampleMenu.addItem("Oversample On", 2);
    //mSampleMenuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, menuId, mSampleMenu);

}
