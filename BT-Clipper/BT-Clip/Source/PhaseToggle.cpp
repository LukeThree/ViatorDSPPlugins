/*
  ==============================================================================

    PhaseToggle.cpp
    Created: 10 Oct 2021 3:19:34pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::set_phase_toggle_props()
{
    m_Phase.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::whitesmoke.withAlpha(0.25f));
    m_Phase.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::skyblue.withAlpha(0.75f));
    m_Phase.setButtonText("Phase");
}
