/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Full_Wave_RectifierAudioProcessorEditor::Full_Wave_RectifierAudioProcessorEditor (Full_Wave_RectifierAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (-1, 3);
    dialShadow.setShadowProperties (shadowProperties);
            
    //Input slider
    addAndMakeVisible(inputSlider);
    inputSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    inputSlider.setRange(-24.0f, 24.0f, 0.25f);
    inputSlider.setTextValueSuffix(" dB");
    inputSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    inputSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    inputSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0.0f));
    inputSlider.setLookAndFeel(&customDial);
    inputSlider.setComponentEffect(&dialShadow);
    inputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputSliderId, inputSlider);
        
    addAndMakeVisible(inputLabel);
    inputLabel.setText("Input", juce::dontSendNotification);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    inputLabel.attachToComponent(&inputSlider, false);
            
    //Trim slider
    addAndMakeVisible(trimSlider);
    trimSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    trimSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    trimSlider.setRange(-24.0f, 24.0f, 0.25f);
    trimSlider.setTextValueSuffix(" dB");
    trimSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    trimSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    trimSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0.0f));
    trimSlider.setLookAndFeel(&customDial);
    trimSlider.setComponentEffect(&dialShadow);
    trimSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
        
    addAndMakeVisible(trimLabel);
    trimLabel.setText("Trim", juce::dontSendNotification);
    trimLabel.setJustificationType(juce::Justification::centred);
    trimLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    trimLabel.attachToComponent(&trimSlider, false);
    
    addAndMakeVisible(windowBorder);
    windowBorder.setText("Full-Wave Rectifier");
    windowBorder.setColour(0x1005400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    windowBorder.setColour(0x1005410, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
        
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(400, 200, 800, 400);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
    setSize (400, 200);
}

Full_Wave_RectifierAudioProcessorEditor::~Full_Wave_RectifierAudioProcessorEditor()
{
}

//==============================================================================
void Full_Wave_RectifierAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = AudioProcessorEditor::getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0), AudioProcessorEditor::getHeight() * 0.01f, juce::Colour::fromFloatRGBA(0.1f, 0.12f, 0.16f, 1.0), AudioProcessorEditor::getHeight() * 0.75f));
    g.fillRect(background);
}

void Full_Wave_RectifierAudioProcessorEditor::resized()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();
                    
    //first column of gui
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::stretch;
                   
    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 2, bounds.getHeight() / 1.25, inputSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .25, 0, 0, 0)));
                   
    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromLeft(bounds.getWidth() / 2));
    /* ============================================================================ */
               
    //second column of gui
    juce::FlexBox flexboxColumnTwo;
    flexboxColumnTwo.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnTwo.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnTwo.alignContent = juce::FlexBox::AlignContent::stretch;
                   
    juce::Array<juce::FlexItem> itemArrayColumnTwo;
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 2, bounds.getHeight() / 1.25, trimSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .25, 0, 0, 0)));
                   
    flexboxColumnTwo.items = itemArrayColumnTwo;
    flexboxColumnTwo.performLayout(bounds.removeFromLeft(bounds.getWidth()));
        
    windowBorder.setBounds(AudioProcessorEditor::getWidth() * .01, AudioProcessorEditor::getHeight() * 0.04, AudioProcessorEditor::getWidth() * .98, AudioProcessorEditor::getHeight() * .90);
}
