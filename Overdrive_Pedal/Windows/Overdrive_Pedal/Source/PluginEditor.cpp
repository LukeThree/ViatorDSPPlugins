/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ViatorDial.h"


//==============================================================================
OverdrivePedalAudioProcessorEditor::OverdrivePedalAudioProcessorEditor (OverdrivePedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    shadowProperties.radius = 16;
    shadowProperties.offset = juce::Point<int> (-1, 3);
    dialShadow.setShadowProperties (shadowProperties);
    
    addAndMakeVisible(&inputSlider);
    inputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    inputSlider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.85f));
    inputSlider.setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0.75));
    inputSlider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.65));
    inputSlider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.5));
    inputSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    inputSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    inputSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    inputSlider.setDoubleClickReturnValue(true, 0.0);
    inputSlider.setTextValueSuffix(" dB");
    inputSlider.setComponentEffect(&dialShadow);
    inputAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputSliderId, inputSlider);
    inputSlider.setRange(-12.0f, 12.0f, 0.5);
    
    addAndMakeVisible(&inputSliderLabel);
    inputSliderLabel.setText("Input", juce::dontSendNotification);
    inputSliderLabel.setJustificationType(juce::Justification::centred);
    inputSliderLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    inputSliderLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    inputSliderLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    inputSliderLabel.attachToComponent(&inputSlider, false);
    
    addAndMakeVisible(&driveSlider);
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 96, 32);
    driveSlider.setTextValueSuffix(" dB");
    driveSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    driveSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    driveSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    driveSlider.setDoubleClickReturnValue(true, 0.0);
    driveSlider.setLookAndFeel(&customDial);
    driveSlider.setComponentEffect(&dialShadow);
    driveAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);
    driveSlider.setRange(0.0, 12.0, 0.25);
    
    addAndMakeVisible(&driveSliderLabel);
    driveSliderLabel.setText("Drive", juce::dontSendNotification);
    driveSliderLabel.setJustificationType(juce::Justification::centred);
    driveSliderLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    driveSliderLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    driveSliderLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    driveSliderLabel.attachToComponent(&driveSlider, false);
    
    addAndMakeVisible(&toneSlider);
    toneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 96, 32);
    toneSlider.setTextValueSuffix(" dB");
    toneSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    toneSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    toneSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    toneSlider.setDoubleClickReturnValue(true, 0.0);
    toneSlider.setLookAndFeel(&customDial);
    toneSlider.setComponentEffect(&dialShadow);
    toneAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, toneSliderId, toneSlider);
    toneSlider.setRange(-6.0, 6.0, 0.25);
    
    addAndMakeVisible(&toneSliderLabel);
    toneSliderLabel.setText("Tone", juce::dontSendNotification);
    toneSliderLabel.setJustificationType(juce::Justification::centred);
    toneSliderLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    toneSliderLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    toneSliderLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    toneSliderLabel.attachToComponent(&toneSlider, false);
    
    addAndMakeVisible(&trimSlider);
    trimSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    trimSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    trimSlider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.85f));
    trimSlider.setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0.75));
    trimSlider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.65));
    trimSlider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 0.5));
    trimSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    trimSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    trimSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    trimSlider.setDoubleClickReturnValue(true, 0.0);
    trimSlider.setTextValueSuffix(" dB");
    trimSlider.setComponentEffect(&dialShadow);
    trimAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
    trimSlider.setRange(-24.0f, 24.0f, 0.5);

    
    addAndMakeVisible(&trimSliderLabel);
    trimSliderLabel.setText("Trim", juce::dontSendNotification);
    trimSliderLabel.setJustificationType(juce::Justification::centred);
    trimSliderLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    trimSliderLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    trimSliderLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    trimSliderLabel.attachToComponent(&trimSlider, false);
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(375, 250, 600, 400);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(1.5);
    setSize (450, 300);
}

OverdrivePedalAudioProcessorEditor::~OverdrivePedalAudioProcessorEditor()
{
    inputSlider.setLookAndFeel(nullptr);
    driveSlider.setLookAndFeel(nullptr);
    toneSlider.setLookAndFeel(nullptr);
    trimSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void OverdrivePedalAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = AudioProcessorEditor::getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(28.0f / 255.0f, 30.0f / 255.0f, 32.0f / 255.0f, 1.0f), 300 * 0.05, juce::Colour::fromFloatRGBA(30.0f / 255.0f, 32.0f / 255.0f, 34.0f / 255.0f, 1.0f), 300 * 0.85));
    g.fillRect(background);
    g.setColour (juce::Colours::white.withAlpha(0.25f));
    g.setFont (14.0f);
    g.drawFittedText ("Algorithms by Landon Viator", 0, 12, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::centredTop, 1);
}

void OverdrivePedalAudioProcessorEditor::resized()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();

    //first column of gui
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth(), bounds.getHeight() / 1.25, inputSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .2, 0, 0, 0)));


    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromLeft(bounds.getWidth() / 3));
    /* ============================================================================*/
    
    //Second column of gui
    juce::FlexBox flexboxColumnTwo;
    flexboxColumnTwo.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnTwo.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnTwo.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnTwo;
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, driveSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .23, 0, 0, 0)));
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, toneSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .1, 0, 0, 0)));


    flexboxColumnTwo.items = itemArrayColumnTwo;
    flexboxColumnTwo.performLayout(bounds.removeFromLeft(bounds.getWidth() / 2));
    /* ============================================================================*/
    
    //Third column of gui
    juce::FlexBox flexboxColumnThree;
    flexboxColumnThree.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnThree.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnThree.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnThree;
    itemArrayColumnThree.add(juce::FlexItem(bounds.getWidth(), bounds.getHeight() / 1.25, trimSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .2, 0, 0, 0)));


    flexboxColumnThree.items = itemArrayColumnThree;
    flexboxColumnThree.performLayout(bounds.removeFromLeft(bounds.getWidth()));
    /* ============================================================================*/
        
}

void OverdrivePedalAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    
}
