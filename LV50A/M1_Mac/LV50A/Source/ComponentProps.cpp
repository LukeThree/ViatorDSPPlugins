/*
  ==============================================================================

    ComponentProps.cpp
    Created: 14 May 2021 9:07:19pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void LV50AAudioProcessorEditor::setShadowProps()
{
    shadowProperties.radius = 8;
    shadowProperties.offset = juce::Point<int> (-2, 6);
    dialShadow.setShadowProperties (shadowProperties);
    
    sliderShadowProperties.radius = 8;
    sliderShadowProperties.offset = juce::Point<int> (0, 0);
    sliderShadowProperties.colour = juce::Colours::black.withAlpha(1.0f);
    sliderShadow.setShadowProperties (sliderShadowProperties);
}

void LV50AAudioProcessorEditor::setCommonVerticalSliderProps(juce::Slider& slider)
{
    slider.setTextValueSuffix(" dB");
    slider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    slider.setColour(0x1001300, juce::Colours::whitesmoke.darker(1.0f).darker(1.0f));
    slider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.65));
    slider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.5));
    slider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    slider.setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    slider.setDoubleClickReturnValue(true, 0.0);
    slider.setComponentEffect(&dialShadow);
    addAndMakeVisible(slider);
}

void LV50AAudioProcessorEditor::setCommonFilterGainSliderProps(juce::Slider& slider)
{
    slider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    slider.setColour(0x1001300, juce::Colours::whitesmoke.darker());
    slider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.65));
    slider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.5));
    slider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    slider.setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setTextValueSuffix(" dB");
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 128, 32);
    slider.setDoubleClickReturnValue(true, 0.0);
    slider.setLookAndFeel(&customDial);
    slider.setComponentEffect(&dialShadow);
    addAndMakeVisible(slider);
}

void LV50AAudioProcessorEditor::setCommonFilterFreqSliderProps(juce::Slider& slider)
{
    slider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    slider.setColour(0x1001300, juce::Colours::whitesmoke.darker());
    slider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.65));
    slider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.5));
    slider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    slider.setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setTextValueSuffix(" Hz");
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    slider.setDoubleClickReturnValue(true, 0.0);
    slider.setLookAndFeel(&customDial2);
    slider.setComponentEffect(&dialShadow);
    addAndMakeVisible(slider);
}

void LV50AAudioProcessorEditor::setCommonLabelProps(juce::Label &label)
{
    addAndMakeVisible(&label);
    label.setJustificationType(juce::Justification::centredTop);
    label.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    label.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5));
    label.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
}

void LV50AAudioProcessorEditor::setCommonButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(&button);
    button.setColour(0x1000100, juce::Colours::whitesmoke.darker(1.0).withAlpha(0.35f));
    button.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    button.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker());
    button.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    button.setColour(0x1000103, juce::Colours::black.brighter(0.1));
}

void LV50AAudioProcessorEditor::setAllProps()
{
    // Set all common slider properties
    for (int i = 0; i < verticalSliders.size(); ++i) {
        
        setCommonVerticalSliderProps(*verticalSliders[i]);
    }
    
    for (int i = 0; i < filterSliders.size(); ++i) {
        
        setCommonFilterGainSliderProps(*filterSliders[i]);
    }
    
    for (int i = 0; i < filterFreqSliders.size(); ++i) {
        
        setCommonFilterFreqSliderProps(*filterFreqSliders[i]);
    }
    
    // Set all common label properties
    for (int i = 0; i < outLabels.size(); ++i) {
        
        setCommonLabelProps(*outLabels[i]);
        outLabels[i]->attachToComponent(verticalSliders[i], false);
        outLabels[i]->setText(outLabelTexts[i], juce::dontSendNotification);
    }
    
    for (int i = 0; i < filterLabels.size(); ++i) {
        
        setCommonLabelProps(*filterLabels[i]);
        filterLabels[i]->setText(filterLabelsTexts[i], juce::dontSendNotification);
        filterLabels[i]->attachToComponent(filterFreqSliders[i], false);
    }
    
    // Set all common button properties
    for (int i = 0; i < buttons.size(); ++i) {
        
        setCommonButtonProps(*buttons[i]);
    }
}
