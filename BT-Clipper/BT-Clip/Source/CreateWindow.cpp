/*
  ==============================================================================

    CreateWindow.cpp
    Created: 9 Oct 2021 12:48:58am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::set_up_window(BTClipAudioProcessor &audioProcessor)
{
    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
    
    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();
    
    // Plugin window will always initialize to half the screen width by half of that for a rectangle
    auto width = x / 2.0;
    auto height = width / 2.0;
        
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width * 0.75, height * 0.75, width * 1.25, height * 1.25);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
        
    if (audioProcessor.m_WindowWidth != 0.0)
    {
        setSize(audioProcessor.m_WindowWidth, audioProcessor.m_WindowHeight);
    }
        
    else
    {
        setSize (width, height);
    }
    
    constructorFinished = true;
}

void BTClipAudioProcessorEditor::save_window_size()
{
    // Save plugin size in value tree
    audioProcessor.variableTree.setProperty("width", getWidth(), nullptr);
    audioProcessor.variableTree.setProperty("height", getHeight(), nullptr);
    
    if (constructorFinished)
    {
        audioProcessor.m_WindowWidth = getWidth();
        audioProcessor.m_WindowHeight = getHeight();
    }
}
