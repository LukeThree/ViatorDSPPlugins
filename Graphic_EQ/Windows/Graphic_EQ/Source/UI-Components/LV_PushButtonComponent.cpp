/*
  ==============================================================================

    LV_PushButtonComponent.cpp
    Created: 24 Oct 2021 8:30:24pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_PushButtonComponent.h"

//==============================================================================
LV_PushButtonComponent::LV_PushButtonComponent(juce::String buttonText)
{
    setClickingTogglesState(true);
    setColour(0x1000100, juce::Colours::whitesmoke.withAlpha(0.5f));
    setColour(0x1000c00, juce::Colours::whitesmoke.withAlpha(0.25f));
    setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    setColour(0x1000102, juce::Colours::black.brighter(0.1));
    setColour(0x1000103, juce::Colours::black.brighter(0.1));
    setButtonText(buttonText);
}

LV_PushButtonComponent::~LV_PushButtonComponent()
{
}

