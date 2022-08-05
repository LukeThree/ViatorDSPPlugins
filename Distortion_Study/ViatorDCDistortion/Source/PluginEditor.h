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

class OtherLookAndFeel : public juce::LookAndFeel_V4{
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
        g.setColour(juce::Colour::fromFloatRGBA(0.20f, 0.22f, 0.24f, .45)); //center
        g.fillEllipse(dialArea);
        g.setColour(juce::Colour::fromFloatRGBA(.2, .77, 1, .75)); //tick color
        
        juce::Path dialTick;
        dialTick.addRectangle(0, -radius + 3, 3.0f, radius * .5);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        g.setColour(juce::Colour::fromFloatRGBA(0, 0, 0, .25)); //outline
        g.drawEllipse(rx, ry, diameter, diameter, 1.0f);
    }
};

class ViatorDCDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    ViatorDCDistortionAudioProcessorEditor (ViatorDCDistortionAudioProcessor&);
    ~ViatorDCDistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::Slider inputSlider, driveSlider, trimSlider;
    std::vector<juce::Slider*> sliders;
    
    juce::Slider inputSliderTrack, driveSliderTrack, trimSliderTrack;
    std::vector<juce::Slider*> tracks;
    
    juce::Label inputSliderLabel, driveSliderLabel, trimSliderLabel;
    std::vector<juce::Label*> labels;
    
    std::string inputSliderLabelText = "Input";
    std::string driveSliderLabelText = "Drive";
    std::string trimSliderLabelText = "Trim";
    std::vector<std::string> labelTexts;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> inputSliderAttach, driveSliderAttach, trimSliderAttach;
    
    OtherLookAndFeel otherLookAndFeel;
    ViatorDCDistortionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViatorDCDistortionAudioProcessorEditor)
};
