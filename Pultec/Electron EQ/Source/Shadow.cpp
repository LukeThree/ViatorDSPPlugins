/*
  ==============================================================================

    Shadow.cpp
    Created: 13 Jul 2021 8:01:37pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void ElectronEQAudioProcessorEditor::setShadowProps()
{
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (-1, 4);
    shadowProperties.colour = juce::Colours::black.withAlpha(0.5f);
    dialShadow.setShadowProperties (shadowProperties);
    
    sliderShadowProperties.radius = 8;
    sliderShadowProperties.offset = juce::Point<int> (0, 0);
    sliderShadowProperties.colour = juce::Colours::black.withAlpha(0.5f);
    sliderShadow.setShadowProperties (sliderShadowProperties);
}
