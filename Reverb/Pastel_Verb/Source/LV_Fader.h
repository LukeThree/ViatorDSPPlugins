/*
  ==============================================================================

    LV_Fader.h
    Created: 21 Jul 2021 12:58:40pm
    Author:  landon viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace juce
{

class LV_Fader : public Slider
{
public:
    
    LV_Fader()
    {
        setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        setTextBoxStyle(juce::Slider::TextBoxBelow, true, 72, 32);
        setRange(-24.0, 24.0, 0.25);
        setDoubleClickReturnValue(true, 0.0);
        setTextValueSuffix(" dB");
        setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromRGB(15, 15, 15));
        setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(15, 15, 15));
        setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
        setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    }
    
private:

    
    
};
}
