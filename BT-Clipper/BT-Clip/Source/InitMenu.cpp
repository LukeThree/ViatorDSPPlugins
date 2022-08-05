/*
  ==============================================================================

    InitMenu.cpp
    Created: 10 Oct 2021 1:46:17pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::init_Menu()
{
    addAndMakeVisible(m_ClipMenu);
    m_ClipMenu.addItem("Hard", 1);
    m_ClipMenu.addItem("Soft", 2);
    m_ClipMenu.addItem("Analog", 3);
    menuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, driveModelId, m_ClipMenu);
}
