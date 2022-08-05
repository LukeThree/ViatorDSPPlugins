/*
  ==============================================================================

    CompressorButtons.cpp
    Created: 22 Jun 2021 10:48:36pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setCompButtonProps()
{
    compressorToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, compressorEnableId, compressorToggle);
    compressorToggle.setButtonText("In");
    compressorToggle.setClickingTogglesState(true);
    
    hpfSideChainToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, hpfEnabledId, hpfSideChainToggle);
    hpfSideChainToggle.setButtonText("Hpf Sc");
    hpfSideChainToggle.setClickingTogglesState(true);

    fastModeToggle.setButtonText("Fast");
    fastModeToggle.setClickingTogglesState(true);
    
    gainBlendLinkToggle.setEnabled(false);
    gainBlendLinkToggle.setVisible(false);
    
    preEQToggle.setButtonText(" Pre EQ");
    preEQToggle.setClickingTogglesState(true);
}
