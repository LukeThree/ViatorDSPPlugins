/*
  ==============================================================================

    BordersProps.cpp
    Created: 22 Jun 2021 11:24:43pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setBorderProps(juce::GroupComponent &border)
{
    addAndMakeVisible(border);
    border.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    border.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    border.setTextLabelPosition(juce::Justification::centred);
    
    inputBorder.setText("Input");
    eqBorder.setText("EQ");
    compressorBorder.setText("Compressor");
    outputBorder.setText("Filter/Out");
}
