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
        
        float diameter = fmin(width, height);
        float radius = diameter * 0.5;
        float centerX = x + width * 0.5;
        float centerY = y + height * 0.5;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        juce::Rectangle<float> dialArea (rx, ry, diameter, diameter);
        g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0).brighter(0.05), 0, juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0).brighter(0.1), 0));

        g.fillEllipse(dialArea);
        g.setColour(juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.75f)); //tick color

        juce::Path dialTick;
        dialTick.addRectangle(0, -radius + 3, 3.0f, radius * 0.452211);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        g.setColour(juce::Colour::fromFloatRGBA(0, 0, 0, 0.25)); //outline
        g.drawEllipse(rx, ry, diameter, diameter, 1.0f);
    }
};
