/*
  ==============================================================================

    DialProps.cpp
    Created: 20 Jun 2021 4:05:13pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setCommonDialProps(juce::Slider &slider)
{
    slider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.85f));
    slider.setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0.75));
    slider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.65));
    slider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.5));
    slider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    slider.setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    slider.setDoubleClickReturnValue(true, 0.0);
    slider.setLookAndFeel(&customDial);
    slider.setComponentEffect(&dialShadow);
    addAndMakeVisible(slider);
}
