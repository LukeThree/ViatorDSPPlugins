/*
  ==============================================================================

    LV_Toggle.h
    Created: 21 Aug 2021 2:06:36am
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace juce
{

class LV_Toggle : public TextButton
{
public:
    
    LV_Toggle()
    {
        setClickingTogglesState(true);
        setColour(0x1000100, juce::Colours::black.withAlpha(0.35f));
        setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
        setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
        setColour(0x1000102, juce::Colours::whitesmoke.withAlpha(0.25f));
        setColour(0x1000103, juce::Colours::black.brighter(0.1));
    }
    
private:
    
};
}
