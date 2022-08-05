/*
  ==============================================================================

    CreateWindow.cpp
    Created: 20 Aug 2021 8:00:23pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::setUpWindow(Pastel_VerbAudioProcessor &audioProcessor)
{
    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getTotalBounds(true);
    
    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();
    
    // Plugin window will always initialize to half the screen width by half of that for a rectangle
    auto width = x / 2.0;
    auto height = width / 2.0;
        
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width / 2.0, height / 2.0, width * 1.5, height * 1.5);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
        
    // If the window size has been saved by the host before
    if (audioProcessor.windowWidth != 0.0)
    {
        setSize(audioProcessor.windowWidth, audioProcessor.windowHeight);
    }
    
    // If this is the first time instanciating the plugin
    else
    {
        setSize (width, height);
    }
    
    constructorFinished = true;
}
