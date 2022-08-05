/*
  ==============================================================================

    ViatorDial.h
    Created: 12 Feb 2021 5:54:44pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/

class ViatorDial : public juce::LookAndFeel_V4{
public:
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override{
        
        float diameter = fmin(width, height) * .9;
        float radius = diameter * 0.5;
        float centerX = x + width * 0.5;
        float centerY = y + height * 0.5;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        juce::Rectangle<float> dialArea (rx, ry, diameter, diameter);
        g.setColour(juce::Colours::palevioletred.darker(1.0).darker(0.3)); //center
        g.fillEllipse(dialArea);
        
        g.setColour(juce::Colours::black.brighter(0.5f).withAlpha(0.75f)); //outline
        g.drawEllipse(rx, ry, diameter, diameter, 3.0f);
        juce::Path dialTick;
        g.setColour(juce::Colours::black.brighter(1.0f).withAlpha(0.75f)); //tick color
        dialTick.addRectangle(0, -radius + 2, 3.0f, radius * 0.6);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    }
};
