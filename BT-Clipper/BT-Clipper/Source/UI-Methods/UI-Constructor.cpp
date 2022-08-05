/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void BTClipperAudioProcessorEditor::uiConstructor()
{
    addAndMakeVisible(windowComponent);
    
    // Window
    initWindow();
    
    // Header
    addAndMakeVisible(headerComponent);
    
    
    
    // Pre stuff
    addAndMakeVisible(preToneComponent);
    
    // Clipper stuff
    addAndMakeVisible(clipperComponent);
    
    // Post stuff
    addAndMakeVisible(postComponent);
}
