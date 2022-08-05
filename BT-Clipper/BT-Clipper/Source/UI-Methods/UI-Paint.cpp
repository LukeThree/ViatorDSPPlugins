/*
  ==============================================================================

    UI-Paint.cpp
    Created: 24 Oct 2021 1:41:00am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void BTClipperAudioProcessorEditor::uiPaint(juce::Graphics &g)
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    auto leftMargin = width * 0.05;
    auto topMargin = height * 0.2;
    
    // Pre rectangle
    juce::Rectangle<float> prePanel;
    prePanel.setBounds(leftMargin, topMargin, width * 0.24f, height * 0.8f);
    g.setColour(juce::Colours::black.brighter(0.1));
    g.fillRect(prePanel);
}
