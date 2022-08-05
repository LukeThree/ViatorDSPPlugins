/*
  ==============================================================================

    ComponentProps.cpp
    Created: 14 May 2021 9:07:19pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void GraphicEQAudioProcessorEditor::setShadowProps()
{
    shadowProperties.radius = 8;
    shadowProperties.offset = juce::Point<int> (0, 0);
    shadowProperties.colour = juce::Colours::black.withAlpha(1.0f);
    dialShadow.setShadowProperties (shadowProperties);
}

void GraphicEQAudioProcessorEditor::setCommonSliderProps(juce::Slider& slider)
{
    slider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    slider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0, 0, 0, 0.0f));
    slider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.0));
    slider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    slider.setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 1.0f).darker(1.0));
    slider.setTextValueSuffix(" dB");
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    slider.setDoubleClickReturnValue(true, 0.0);
    slider.setLookAndFeel(&customFader);
    slider.setComponentEffect(&dialShadow);
    addAndMakeVisible(slider);
}

void GraphicEQAudioProcessorEditor::setCommonLabelProps(juce::Label &label)
{
    addAndMakeVisible(&label);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    label.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    label.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
}

void GraphicEQAudioProcessorEditor::setCommonButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(&button);
    button.setColour(0x1000100, juce::Colour::fromFloatRGBA(0, 0, 0, .5));
    button.setClickingTogglesState(true);
    button.setColour(0x1000100, juce::Colours::whitesmoke.withAlpha(0.5f));
    button.setColour(0x1000c00, juce::Colours::whitesmoke.withAlpha(0.25f));
    button.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    button.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    button.setColour(0x1000103, juce::Colours::black.brighter(0.1));
}

void GraphicEQAudioProcessorEditor::setAllProps()
{
    // Set all common slider properties
    for (int i = 0; i < sliders.size(); ++i)
    {
        setCommonSliderProps(*sliders[i]);
    }
    
    // Set all common label properties
    for (int i = 0; i < labels.size(); ++i)
    {
        setCommonLabelProps(*labels[i]);
        labels[i]->attachToComponent(sliders[i], false);
        labels[i]->setText(labelTexts[i], juce::dontSendNotification);
    }
    
    // Set all common button properties
    for (int i = 0; i < buttons.size(); ++i)
    {
        setCommonButtonProps(*buttons[i]);
        buttons[i]->setButtonText(buttonTexts[i]);
    }
}
