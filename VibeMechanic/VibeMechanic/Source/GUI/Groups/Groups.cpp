/*
  ==============================================================================

    Groups.cpp
    Created: 11 Jun 2022 11:32:10pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void  VibeMechinicAudioProcessorEditor::setGroupProps(juce::GroupComponent& group)
{
    addAndMakeVisible(group);
    group.setColour(juce::GroupComponent::ColourIds::outlineColourId, m_textAccentColor);
    group.setColour(juce::GroupComponent::ColourIds::textColourId, m_textAccentColor);

}
