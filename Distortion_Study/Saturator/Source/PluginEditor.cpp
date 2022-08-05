/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessorEditor::SaturatorAudioProcessorEditor (SaturatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (-1, 3);
    dialShadow.setShadowProperties (shadowProperties);
    
    //Drive slider
    addAndMakeVisible(driveSlider);
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    driveSlider.setRange(0, 24, 0.5);
    driveSlider.setTextValueSuffix(" dB");
    driveSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    driveSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    driveSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    driveSlider.setLookAndFeel(&customDial);
    driveSlider.setComponentEffect(&dialShadow);
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);
    
    //Trim slider
    addAndMakeVisible(trimSlider);
    trimSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    trimSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    trimSlider.setRange(-36, 36, 0.5);
    trimSlider.setTextValueSuffix(" dB");
    trimSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    trimSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    trimSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    trimSlider.setLookAndFeel(&customDial);
    trimSlider.setComponentEffect(&dialShadow);
    trimSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
    
    //Combo box
    addAndMakeVisible(distortionType);
    distortionType.setTextWhenNothingSelected("Distortion Model");
    distortionType.setColour(0x1000a00, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    distortionType.setColour(0x1000b00, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    distortionType.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    distortionType.setColour(0x1000e00, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.25f));
    distortionType.addItem("Distortion Model", 1);
    distortionType.addItem("Soft Clip", 2);
    distortionType.addItem("Hard Clip", 3);
    distortionType.addItem("DC", 4);
    distortionType.addItem("Diode", 5);
    distortionType.addItem("Full-Wave Rect", 6);
    distortionType.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
    distortionType.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::black);
    distortionTypeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, modelId, distortionType);
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(300, 400, 450, 600);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(0.75);
    setSize (300, 400);
}

SaturatorAudioProcessorEditor::~SaturatorAudioProcessorEditor()
{
}

//==============================================================================
void SaturatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromFloatRGBA(0.1f, 0.12f, 0.16f, 1.0));
            
    juce::Rectangle<int> background = AudioProcessorEditor::getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0), 300 * .01, juce::Colour::fromFloatRGBA(0.12f, 0.14f, 0.18f, 1.0), 300 * .99));
    g.fillRect(background);
}

void SaturatorAudioProcessorEditor::resized()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();
            
    //first column of gui
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::center;
            
    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 2, bounds.getHeight() / 10, distortionType).withMargin(juce::FlexItem::Margin(bounds.getWidth() * .05, 0, 0, 0)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 2.5, bounds.getHeight() / 2.5, driveSlider).withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 2.5, bounds.getHeight() / 2.5, trimSlider).withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));

    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromLeft(bounds.getWidth()));
}
