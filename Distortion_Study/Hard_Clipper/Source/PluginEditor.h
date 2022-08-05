/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

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
        
        float diameter = fmin(width, height) * 0.65;
        float radius = diameter * 0.5;
        float centerX = x + width * 0.5;
        float centerY = y + height * 0.5;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        juce::Rectangle<float> dialArea (rx, ry, diameter, diameter);
        g.setColour(juce::Colour::fromFloatRGBA(0.15, 0.15, 0.15, 1)); //center
        g.setGradientFill(juce::ColourGradient::horizontal(juce::Colour::fromFloatRGBA(0.1, 0.1, 0.1, 1), centerY * .25, juce::Colour::fromFloatRGBA(0.15, 0.15, 0.15, 1), centerY * 2.5));
        g.fillEllipse(dialArea);
        g.setColour(juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.5f)); //tick color

        juce::Path dialTick;
        dialTick.addRectangle(0, -radius + 3, 3.0f, radius * 0.452211);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        g.setColour(juce::Colour::fromFloatRGBA(0, 0, 0, .25)); //outline
        g.drawEllipse(rx, ry, diameter, diameter, 1.0f);
    }
};

class Hard_ClipperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Hard_ClipperAudioProcessorEditor (Hard_ClipperAudioProcessor&);
    ~Hard_ClipperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    ViatorDial customDial;
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
            
    juce::Slider inputSlider, trimSlider;
    juce::Label inputLabel, trimLabel;
    juce::GroupComponent windowBorder;
            
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> inputSliderAttach, trimSliderAttach;
    
    Hard_ClipperAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hard_ClipperAudioProcessorEditor)
};
