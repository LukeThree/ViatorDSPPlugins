/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void BTClipperAudioProcessorEditor::uiResized(float width, float height)
{
    auto leftMargin = height * 0.02;
    auto topMargin = height * 0.1;
    
    // Plugin background UI
    windowComponent.setWidthAndHeight(width, height);
    windowComponent.setBounds(getLocalBounds());
    
    headerComponent.setWidthAndHeight(width, height);
    headerComponent.setBounds(0, 0, width, height * 0.08f);
    
    // Pre Tone
    preToneComponent.setWidthAndHeight(width, height);
    preToneComponent.setBounds(leftMargin, topMargin, width * 0.24f, height * 0.88f);
    
    // Clip Stuff
    clipperComponent.setWidthAndHeight(width, height);
    clipperComponent.setBounds(preToneComponent.getX() + preToneComponent.getWidth() + leftMargin, topMargin, width * 0.48f, height * 0.88f);
    
    // Post Stuff
    postComponent.setWidthAndHeight(width, height);
    postComponent.setBounds(clipperComponent.getX() + clipperComponent.getWidth() + leftMargin, topMargin, width * 0.24f, height * 0.88f);
    
    // Save plugin size in the tree
    saveWindowSize();
}
