/*
  ==============================================================================

    PluginBackground.cpp
    Created: 20 Jun 2021 4:28:54pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setBackgroundProps(juce::Graphics &g)
{
    //Image layer from Illustrator
    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::pluginBackground_png, BinaryData::pluginBackground_pngSize);
    g.drawImageWithin(pluginBackground, 0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::RectanglePlacement::stretchToFit);
        
    // Background overlay
    juce::Rectangle<float> backgroundDarker;
    backgroundDarker.setBounds(0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight());
    g.setColour(juce::Colour::fromFloatRGBA(0.09f, 0.10f, 0.12f, 0.65f));
    g.fillRect(backgroundDarker);
    
    vuMeter = juce::ImageCache::getFromMemory(BinaryData::VuMeter_png, BinaryData::VuMeter_pngSize);
    g.drawImageWithin(vuMeter, AudioProcessorEditor::getWidth() / 3.4, AudioProcessorEditor::getWidth() * .18, AudioProcessorEditor::getWidth() / 3, AudioProcessorEditor::getWidth() / 6, juce::RectanglePlacement::stretchToFit);
    

}
