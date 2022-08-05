/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Pedal_iR_PrototyperAudioProcessorEditor::Pedal_iR_PrototyperAudioProcessorEditor (Pedal_iR_PrototyperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (-1, 3);
    dialShadow.setShadowProperties (shadowProperties);
                
    //Input slider
    addAndMakeVisible(inputSlider);
    inputSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 72, 32);
    inputSlider.setTextValueSuffix(" dB");
    inputSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
    inputSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
    inputSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    inputSlider.setLookAndFeel(&customDial);
    inputSlider.setComponentEffect(&dialShadow);
    inputSlider.setRange(0.0, 24.0, 0.25);
    inputSlider.setDoubleClickReturnValue(true, 0.0);
    inputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputSliderId, inputSlider);
    
    addAndMakeVisible(toneSlider);
    toneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 72, 32);
    toneSlider.setTextValueSuffix(" dB");
    toneSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
    toneSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
    toneSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    toneSlider.setLookAndFeel(&customDial);
    toneSlider.setComponentEffect(&dialShadow);
    toneSlider.setRange(-9.0, 9.0, 0.25);
    toneSlider.setDoubleClickReturnValue(true, 0.0);
    toneSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, toneSliderId, toneSlider);
                
    //Trim slider
    addAndMakeVisible(trimSlider);
    trimSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    trimSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 72, 32);
    trimSlider.setTextValueSuffix(" dB");
    trimSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
    trimSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
    trimSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    trimSlider.setLookAndFeel(&customDial);
    trimSlider.setComponentEffect(&dialShadow);
    trimSlider.setRange(-24.0, 24.0, 0.25);
    trimSlider.setDoubleClickReturnValue(true, 0.0);
    trimSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
    
    setSize (400, 300);
}

Pedal_iR_PrototyperAudioProcessorEditor::~Pedal_iR_PrototyperAudioProcessorEditor()
{
    inputSlider.setLookAndFeel(nullptr);
    toneSlider.setLookAndFeel(nullptr);
    trimSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void Pedal_iR_PrototyperAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Rectangle<int> background = AudioProcessorEditor::getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0), 300 * .01, juce::Colour::fromFloatRGBA(0.12f, 0.14f, 0.18f, 1.0), 300 * .99));
    g.fillRect(background);
    g.setColour (juce::Colours::white.darker(1.0));
    g.setFont (36.0f);
    g.drawFittedText ("Pedal Simulator v2", 0, 24, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::centredTop, 1);
    
    //Image layer from Illustrator
    //plugbackground = juce::ImageCache::getFromMemory(BinaryData::pedal_background_png, BinaryData::pedal_background_pngSize);
    //g.drawImageWithin(plugbackground, 0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::RectanglePlacement::stretchToFit);
}

void Pedal_iR_PrototyperAudioProcessorEditor::resized()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();
                        
    //first column of gui
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::row;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::stretch;
                       
    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 3.5, bounds.getHeight() / 3.5, inputSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .15, 0, 0, bounds.getHeight() * .05)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 3.5, bounds.getHeight() / 3.5, toneSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .15, 0, 0, bounds.getHeight() * .05)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 3.5, bounds.getHeight() / 3.5, trimSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .15, 0, 0, bounds.getHeight() * .05)));
                       
    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromLeft(bounds.getWidth()));
}
