/*
  ==============================================================================

    InitToggles.cpp
    Created: 9 Oct 2021 2:40:08am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"



void BTClipAudioProcessorEditor::init_toggles()
{
    addAndMakeVisible(m_PrePostToggle);
    filterEngageButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, filterToggleId, m_PrePostToggle);

    addAndMakeVisible(m_Phase);
    phaseButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseId, m_Phase);
    
    set_pre_post_toggle_props();
}
