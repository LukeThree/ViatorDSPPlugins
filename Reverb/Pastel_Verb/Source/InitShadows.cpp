/*
  ==============================================================================

    InitShadows.cpp
    Created: 21 Aug 2021 1:40:33am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::initShadows()
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
