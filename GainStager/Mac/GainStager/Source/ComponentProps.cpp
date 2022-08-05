/*
  ==============================================================================

    ComponentProps.cpp
    Created: 14 May 2021 9:07:19pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void GainStagerAudioProcessorEditor::setShadowProps()
{
    shadowProperties.radius = 8;
    shadowProperties.offset = juce::Point<int> (-1, 3);
    dialShadow.setShadowProperties (shadowProperties);
}

void GainStagerAudioProcessorEditor::setCommonSliderProps(juce::Slider& slider)
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

void GainStagerAudioProcessorEditor::setCommonLabelProps(juce::Label &label)
{
    addAndMakeVisible(&label);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    label.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    label.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
}

void GainStagerAudioProcessorEditor::setCommonButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(&button);
    button.setColour(0x1000100, juce::Colours::whitesmoke.withAlpha(0.5f));
    button.setColour(0x1000c00, juce::Colours::whitesmoke.withAlpha(0.25f));
    button.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    button.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    button.setColour(0x1000103, juce::Colours::black.brighter(0.1));
}

void GainStagerAudioProcessorEditor::setAllProps()
{
    // Set all common slider properties
    for (int i = 0; i < sliders.size(); ++i) {
        
        setCommonSliderProps(*sliders[i]);
    }
    
    // Set all common label properties
    for (int i = 0; i < labels.size(); ++i) {
        
        setCommonLabelProps(*labels[i]);
    }
    
    // Set all common button properties
    for (int i = 0; i < buttons.size(); ++i) {
        
        setCommonButtonProps(*buttons[i]);
    }
}
