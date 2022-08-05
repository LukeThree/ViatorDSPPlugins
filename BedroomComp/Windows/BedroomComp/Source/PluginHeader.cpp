/*
  ==============================================================================

    PluginHeader.cpp
    Created: 20 Jun 2021 4:29:22pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setHeaderProps(juce::Graphics& g)
{
    // Header rectangle
    juce::Rectangle<float> header;
    header.setBounds(0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight() / 8);
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillRect(header);
    
    // Set header text
    g.setFont (16.0f);
    g.setColour (juce::Colours::white.withAlpha(0.25f));
    g.drawFittedText ("Algorithms by Landon Viator", 12, header.getHeight() / 3, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::topLeft, 1);
    
    // Logo
    mLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    g.drawImageWithin(mLogo, AudioProcessorEditor::getWidth() * 0.7f, header.getHeight() / 4, AudioProcessorEditor::getWidth() * 0.05f * 4.58, AudioProcessorEditor::getWidth() * 0.05f, juce::RectanglePlacement::centred);
    
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    
    mWebLink.setBounds(AudioProcessorEditor::getWidth() * 0.7f, header.getHeight() / 4, AudioProcessorEditor::getWidth() * 0.05f * 4.58, AudioProcessorEditor::getWidth() * 0.05f);
}
