/*
  ==============================================================================

    SaveWindowSize.cpp
    Created: 20 Aug 2021 9:54:05pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::saveWindowSize()
{
    // Save plugin size in value tree
    audioProcessor.variableTree.setProperty("width", getWidth(), nullptr);
    audioProcessor.variableTree.setProperty("height", getHeight(), nullptr);
    
    // Save the current size of the window to recall opening the plugin after minimizing it
    // There was a bug where resized() started in the middle of the constructor
    // This was causing it to always open very small
    // This if check fixes it
    if (constructorFinished)
    {
        audioProcessor.windowWidth = AudioProcessorEditor::getWidth();
        audioProcessor.windowHeight = AudioProcessorEditor::getHeight();
    }
}
