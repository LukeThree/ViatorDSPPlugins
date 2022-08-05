/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ViatorDial.h"


//==============================================================================
ReverbAudioProcessorEditor::ReverbAudioProcessorEditor (ReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    shadowProperties.radius = 16;
    shadowProperties.offset = juce::Point<int> (-1, 3);
    dialShadow.setShadowProperties (shadowProperties);
    
    addAndMakeVisible(&filterEngageButton);
    filterEngageButton.setClickingTogglesState(true);
    filterEngageButton.setButtonText("LPF");
    filterEngageButton.setColour(0x1000100, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
    filterEngageButton.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    filterEngageButton.setColour(0x1000101, juce::Colour::fromFloatRGBA(0, 0, 0, .15));
    filterEngageButton.setColour(0x1000102, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    filterEngageButton.setColour(0x1000103, juce::Colour::fromFloatRGBA(.23, .77, 1, 0.5));
    filterEngageButton.setColour(0x1000103, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.5f));
    filterEngageButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, filterEngageId, filterEngageButton);

    
    addAndMakeVisible(&filterModeButton);
    filterModeButton.setClickingTogglesState(true);
    filterModeButton.setButtonText("12/24 dB");
    filterModeButton.setColour(0x1000100, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
    filterModeButton.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    filterModeButton.setColour(0x1000101, juce::Colour::fromFloatRGBA(0, 0, 0, .15));
    filterModeButton.setColour(0x1000102, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    filterModeButton.setColour(0x1000103, juce::Colour::fromFloatRGBA(.23, .77, 1, 0.5));
    filterModeButton.setColour(0x1000103, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.5f));
    filterModeButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, filterModeId, filterModeButton);
    
    addAndMakeVisible(&cutoffSlider);
    cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 96, 32);
    cutoffSlider.setTextValueSuffix(" Hz");
    cutoffSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, cutoffSliderId, cutoffSlider);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setSkewFactorFromMidPoint(750);
    cutoffSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    cutoffSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    cutoffSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    cutoffSlider.setDoubleClickReturnValue(true, 0.0);
    cutoffSlider.setLookAndFeel(&customDial);
    cutoffSlider.setComponentEffect(&dialShadow);
    
    addAndMakeVisible(&roomSizeSlider);
    roomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    roomSizeSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    roomSizeSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    roomSizeSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    roomSizeSlider.setDoubleClickReturnValue(true, 0.0);
    roomSizeSlider.setLookAndFeel(&customDial);
    roomSizeSlider.setTextValueSuffix(" %");
    roomSizeSlider.setRange(0.0, 100.0, 1.0);
    roomSizeSlider.setComponentEffect(&dialShadow);
    roomSizeSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, roomSizeSliderId, roomSizeSlider);

    
    addAndMakeVisible(&resonanceSlider);
    resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    resonanceSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    resonanceSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    resonanceSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    resonanceSlider.setDoubleClickReturnValue(true, 0.0);
    resonanceSlider.setLookAndFeel(&customDial);
    resonanceSlider.setTextValueSuffix(" %");
    resonanceSlider.setRange(0.0, 100.0, 1.0);
    resonanceSlider.setComponentEffect(&dialShadow);
    resonanceSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, resonanceSliderId, resonanceSlider);
    
    addAndMakeVisible(&dampingSlider);
    dampingSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    dampingSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    dampingSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    dampingSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    dampingSlider.setDoubleClickReturnValue(true, 0.0);
    dampingSlider.setLookAndFeel(&customDial);
    dampingSlider.setTextValueSuffix(" %");
    dampingSlider.setRange(0.0, 100.0, 1.0);
    dampingSlider.setComponentEffect(&dialShadow);
    dampingSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, dampingSliderId, dampingSlider);

    
    addAndMakeVisible(&driveSlider);
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    driveSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    driveSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    driveSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    driveSlider.setDoubleClickReturnValue(true, 0.0);
    driveSlider.setLookAndFeel(&customDial);
    driveSlider.setTextValueSuffix(" dB");
    driveSlider.setRange(0.0, 24.0, 0.5);
    driveSlider.setComponentEffect(&dialShadow);
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);


    addAndMakeVisible(&widthSlider);
    widthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    widthSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    widthSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    widthSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    widthSlider.setDoubleClickReturnValue(true, 0.0);
    widthSlider.setLookAndFeel(&customDial);
    widthSlider.setTextValueSuffix(" %");
    widthSlider.setRange(0.0, 100.0, 1.0);
    widthSlider.setComponentEffect(&dialShadow);
    widthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, widthSliderId, widthSlider);

    
    addAndMakeVisible(&drySlider);
    drySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    drySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    drySlider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    drySlider.setColour(0x1001300, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 1.0));
    drySlider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0, 0, 0, 0.65));
    drySlider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 1));
    drySlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    drySlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    drySlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    drySlider.setDoubleClickReturnValue(true, 0.0);
    drySlider.setTextValueSuffix(" %");
    drySlider.setRange(0.0, 100.0, 1.0);
    drySlider.setComponentEffect(&dialShadow);
    drySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, drySliderId, drySlider);

    
    addAndMakeVisible(&wetSlider);
    wetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 128, 32);
    wetSlider.setColour(0x1001200, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
    wetSlider.setColour(0x1001300, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 1.0));
    wetSlider.setColour(0x1001310, juce::Colour::fromFloatRGBA(0, 0, 0, 0.65));
    wetSlider.setColour(0x1001311, juce::Colour::fromFloatRGBA(0, 0, 0, 1));
    wetSlider.setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    wetSlider.setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
    wetSlider.setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    wetSlider.setDoubleClickReturnValue(true, 0.0);
    wetSlider.setTextValueSuffix(" %");
    wetSlider.setRange(0.0, 100.0, 1.0);
    wetSlider.setComponentEffect(&dialShadow);
    wetSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, wetSliderId, wetSlider);

    
    addAndMakeVisible(&cutoffLabel);
    cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    cutoffLabel.setJustificationType(juce::Justification::centred);
    cutoffLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    cutoffLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    cutoffLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    
    addAndMakeVisible(&resonanceLabel);
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    resonanceLabel.setJustificationType(juce::Justification::centred);
    resonanceLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    resonanceLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    resonanceLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    resonanceLabel.attachToComponent(&resonanceSlider, false);
    
    addAndMakeVisible(&driveLabel);
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    driveLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    driveLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    driveLabel.attachToComponent(&driveSlider, false);
    
    addAndMakeVisible(&roomLabel);
    roomLabel.setText("Room Size", juce::dontSendNotification);
    roomLabel.setJustificationType(juce::Justification::centred);
    roomLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    roomLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    roomLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    roomLabel.attachToComponent(&roomSizeSlider, false);
    
    addAndMakeVisible(&dampingLabel);
    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::centred);
    dampingLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    dampingLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    dampingLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    dampingLabel.attachToComponent(&dampingSlider, false);
    
    addAndMakeVisible(&widthLabel);
    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centred);
    widthLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    widthLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    widthLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    widthLabel.attachToComponent(&widthSlider, false);
    
    addAndMakeVisible(&dryLabel);
    dryLabel.setText("Dry", juce::dontSendNotification);
    dryLabel.setJustificationType(juce::Justification::centred);
    dryLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    dryLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    dryLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    dryLabel.attachToComponent(&drySlider, false);
    
    addAndMakeVisible(&wetLabel);
    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.setJustificationType(juce::Justification::centred);
    wetLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    wetLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.18));
    wetLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    wetLabel.attachToComponent(&wetSlider, false);
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(750, 250, 1200, 400);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(3.0);
    setSize (900, 300);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    cutoffSlider.setLookAndFeel(nullptr);
    resonanceSlider.setLookAndFeel(nullptr);
    driveSlider.setLookAndFeel(nullptr);
    roomSizeSlider.setLookAndFeel(nullptr);
    dampingSlider.setLookAndFeel(nullptr);
    widthSlider.setLookAndFeel(nullptr);
    drySlider.setLookAndFeel(nullptr);
    wetSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void ReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = AudioProcessorEditor::getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0), 300 * .01, juce::Colour::fromFloatRGBA(0.12f, 0.14f, 0.18f, 1.0), 300 * .99));
    g.fillRect(background);
    g.setColour (juce::Colours::white.withAlpha(0.18f));
    g.setFont (16.0f);
    g.drawFittedText ("Viator Reverb v1.0.0", 12, 12, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::topLeft, 1);
       
}

void ReverbAudioProcessorEditor::resized()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();

    //first column of gui
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 11, bounds.getHeight() / 6, filterEngageButton).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .32, 0, 0, bounds.getWidth() * .05)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 11, bounds.getHeight() / 6, filterModeButton).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .15, 0, 0, bounds.getWidth() * .05)));


    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromLeft(bounds.getWidth() / 6));
    /* ============================================================================*/
    
    //Second column of gui
    juce::FlexBox flexboxColumnTwo;
    flexboxColumnTwo.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnTwo.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnTwo.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnTwo;
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, cutoffSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .16, 0, 0, bounds.getWidth() * .01)));
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, roomSizeSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .1, 0, 0, bounds.getWidth() * .01)));

    flexboxColumnTwo.items = itemArrayColumnTwo;
    flexboxColumnTwo.performLayout(bounds.removeFromLeft(bounds.getWidth() / 5));
    /* ============================================================================*/
    
    //Third column of gui
    juce::FlexBox flexboxColumnThree;
    flexboxColumnThree.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnThree.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnThree.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnThree;
    itemArrayColumnThree.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, resonanceSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .16, 0, 0, bounds.getWidth() * .01)));
    itemArrayColumnThree.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, dampingSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .1, 0, 0, bounds.getWidth() * .01)));


    flexboxColumnThree.items = itemArrayColumnThree;
    flexboxColumnThree.performLayout(bounds.removeFromLeft(bounds.getWidth() / 4));
    /* ============================================================================*/
    
    //Fourth column of gui
    juce::FlexBox flexboxColumnFour;
    flexboxColumnFour.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnFour.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnFour.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnFour;
    itemArrayColumnFour.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, driveSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .16, 0, 0, bounds.getWidth() * .01)));
    itemArrayColumnFour.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 3, widthSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .1, 0, 0, bounds.getWidth() * .01)));


    flexboxColumnFour.items = itemArrayColumnFour;
    flexboxColumnFour.performLayout(bounds.removeFromLeft(bounds.getWidth() / 3));
    /* ============================================================================*/
    
    //Fifth column of gui
    juce::FlexBox flexboxColumnFive;
    flexboxColumnFive.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnFive.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnFive.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnFive;
    itemArrayColumnFive.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 1.25, drySlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .12, 0, 0, bounds.getWidth() * .01)));


    flexboxColumnFive.items = itemArrayColumnFive;
    flexboxColumnFive.performLayout(bounds.removeFromLeft(bounds.getWidth() / 2));
    /* ============================================================================*/
    
    //Sixth column of gui
    juce::FlexBox flexboxColumnSix;
    flexboxColumnSix.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnSix.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnSix.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnSix;
    itemArrayColumnSix.add(juce::FlexItem(bounds.getWidth() / 3, bounds.getHeight() / 1.25, wetSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * .12, 0, 0, bounds.getWidth() * .025)));


    flexboxColumnSix.items = itemArrayColumnSix;
    flexboxColumnSix.performLayout(bounds.removeFromLeft(bounds.getWidth()));
    /* ============================================================================*/
}
