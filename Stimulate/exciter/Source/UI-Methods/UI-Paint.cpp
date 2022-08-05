/*
  ==============================================================================

    UI-Paint.cpp
    Created: 24 Oct 2021 1:41:00am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void ExciterAudioProcessorEditor::uiPaint(juce::Graphics &g)
{
    background = juce::ImageCache::getFromMemory(BinaryData::dark_blue_png, BinaryData::dark_blue_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    
    g.setColour(juce::Colours::whitesmoke);
    g.setFont(juce::Font(18.0f));
    g.drawFittedText("Stimulate v1.4", getWidth() * 0.8, getHeight() * 0.025, getWidth() * 0.125, getHeight() * 0.25, juce::Justification::topLeft, 1);

    drawFrame(g);
}
