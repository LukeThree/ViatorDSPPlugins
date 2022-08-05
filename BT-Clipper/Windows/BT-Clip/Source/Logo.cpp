/*
  ==============================================================================

    Logo.cpp
    Created: 13 Oct 2021 4:38:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::paint_logo(juce::Graphics &g)
{
    // Logo layer
    footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    
    // Draw and position the image
    g.drawImageWithin(footerLogo, AudioProcessorEditor::getWidth() * 0.38, AudioProcessorEditor::getHeight() * 0.8 + 4, AudioProcessorEditor::getWidth() * 0.25, AudioProcessorEditor::getHeight() * 0.1, juce::RectanglePlacement::centred);
    
    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(AudioProcessorEditor::getWidth() * 0.38, AudioProcessorEditor::getHeight() * 0.8 + 4, AudioProcessorEditor::getWidth() * 0.25, AudioProcessorEditor::getHeight() * 0.1);
    
    // Name
    g.setColour (juce::Colours::whitesmoke.withAlpha(0.125f));
    g.setFont(AudioProcessorEditor::getWidth() * 0.015);
    g.drawFittedText ("BT Clipper v1.3.0", AudioProcessorEditor::getWidth() * 0.12, AudioProcessorEditor::getHeight() * 0.12, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::topLeft, 1);
}
