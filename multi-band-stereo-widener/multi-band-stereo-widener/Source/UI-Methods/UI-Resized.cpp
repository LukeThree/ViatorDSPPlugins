/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void MultibandstereowidenerAudioProcessorEditor::uiResized(float width, float height)
{
    // Plugin background UI
    windowComponent.setWidthAndHeight(width, height);
    windowComponent.setBounds(getLocalBounds());
    
    lowWidthDial.setBounds(width * 0.2, width * 0.1, width * 0.1, width * 0.1);
    highWidthDial.setBounds(lowWidthDial.getX() + lowWidthDial.getWidth(), lowWidthDial.getY(), lowWidthDial.getWidth(), lowWidthDial.getHeight());
    crossoverFader.setBounds(lowWidthDial.getX(), lowWidthDial.getY() + lowWidthDial.getHeight(), lowWidthDial.getWidth() * 2.0, lowWidthDial.getHeight());
    
    // Save plugin size in the tree
    saveWindowSize();
}
