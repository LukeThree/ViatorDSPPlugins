/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void JustaFilterAudioProcessorEditor::uiResized(float width, float height)
{
    auto leftMargin = height * 0.02;
    auto topMargin = height * 0.1;
    
    // Plugin background UI
    windowComponent.setWidthAndHeight(width, height);
    windowComponent.setBounds(getLocalBounds());
    
    headerComponent.setWidthAndHeight(width, height);
    headerComponent.setBounds(0, 0, width, height * 0.12f);
    
    filterComponent.setBounds(0, height * 0.12f, width, height * 0.88f);
    
    // Save plugin size in the tree
    saveWindowSize();
}
