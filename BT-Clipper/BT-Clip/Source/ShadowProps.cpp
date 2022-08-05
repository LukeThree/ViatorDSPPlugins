/*
  ==============================================================================

    ShadowProps.cpp
    Created: 10 Oct 2021 4:51:01pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::init_shadows()
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
