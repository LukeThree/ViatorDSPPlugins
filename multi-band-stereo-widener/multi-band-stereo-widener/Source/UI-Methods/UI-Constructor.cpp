/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void MultibandstereowidenerAudioProcessorEditor::uiConstructor()
{
    addAndMakeVisible(windowComponent);
    
    // Window
    initWindow();
    
    lowWidthDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowWidthId, lowWidthDial);
    addAndMakeVisible(lowWidthDial);
    
    highWidthDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highWidthId, highWidthDial);
    addAndMakeVisible(highWidthDial);
    
    
    crossoverDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, crossoverId, crossoverFader);
    crossoverFader.setRange(60.0, 16000.0, 1.0);
    crossoverFader.setSkewFactorFromMidPoint(1000.0);
    addAndMakeVisible(crossoverFader);
}
