/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DiodeAmplifierAudioProcessorEditor::DiodeAmplifierAudioProcessorEditor (DiodeAmplifierAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
        shadowProperties.radius = 24;
        shadowProperties.offset = juce::Point<int> (-1, 3);
        dialShadow.setShadowProperties (shadowProperties);
        
        sliders.reserve(6);
        sliders = {
            &inputSlider, &driveSlider, &lowSlider, &midSlider, &highSlider, &outputSlider
        };
        
        labels.reserve(6);
        labels = {
                 &inputLabel, &driveLabel, &lowLabel, &midLabel, &highLabel, &outputLabel
            };
            
        labelTexts.reserve(6);
        labelTexts = {
            inputSliderLabelText, driveSliderLabelText, lowSliderLabelText, midSliderLabelText, highSliderLabelText, outputSliderLabelText
        };
            
    inputSlider.setLookAndFeel(&customDial);
    driveSlider.setLookAndFeel(&customDial);
    outputSlider.setLookAndFeel(&customDial);
    lowSlider.setLookAndFeel(&customDial2);
    midSlider.setLookAndFeel(&customDial2);
    highSlider.setLookAndFeel(&customDial2);

        for (auto i = 0; i < sliders.size(); i++) {
            addAndMakeVisible(sliders[i]);
            sliders[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
            sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
            sliders[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
            sliders[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
            sliders[i]->setColour(0x1001500, juce::Colour::fromFloatRGBA(0, 0, 0, 0.25f));
            sliders[i]->setComponentEffect(&dialShadow);
            sliders[i]->setRange(-24.0, 24.0, 0.25);
            sliders[i]->setDoubleClickReturnValue(true, 0.0);
        }
        
        driveSlider.setRange(0.0, 10.0, 0.25);
        lowSlider.setRange(-6.0, 6.0, 0.25);
        midSlider.setRange(-6.0, 6.0, 0.25);
        highSlider.setRange(-6.0, 6.0, 0.25);
        
        inputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputGainSliderId, inputSlider);
        driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);
        lowSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowSliderId, lowSlider);
        midSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, midSliderId, midSlider);
        highSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highSliderId, highSlider);
        outputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputGainSliderId, outputSlider);
        
        for (auto i = 0; i < labels.size(); i++)
            {
                addAndMakeVisible(labels[i]);
                labels[i]->setText(labelTexts[i], juce::dontSendNotification);
                labels[i]->setJustificationType(juce::Justification::centred);
                labels[i]->setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
                labels[i]->attachToComponent(sliders[i], false);
            }
        
        addAndMakeVisible(windowBorder);
        windowBorder.setText("Ignorant Diode Amplifier");
        windowBorder.setColour(0x1005400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
        windowBorder.setColour(0x1005410, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
        
    addAndMakeVisible(&brightButton);
    brightButton.setButtonText("Bright");
    brightButton.setClickingTogglesState(true);
    brightButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, brightId, brightButton);
    brightButton.setColour(0x1000100, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));
    brightButton.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    brightButton.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    brightButton.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    brightButton.setColour(0x1000103, juce::Colours::black.brighter(0.1));

    addAndMakeVisible(&cabButton);
    cabButton.setButtonText("Load IR");
    cabButton.setColour(0x1000100, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));
    cabButton.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    cabButton.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    cabButton.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    cabButton.setColour(0x1000103, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(&cabToggleButton);
    cabToggleButton.setClickingTogglesState(true);
    cabToggleButton.setToggleState(true, juce::dontSendNotification);
    cabToggleButton.setButtonText("Cab On");
    cabToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, cabId, cabToggleButton);
    cabToggleButton.setColour(0x1000100, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));
    cabToggleButton.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    cabToggleButton.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    cabToggleButton.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    cabToggleButton.setColour(0x1000103, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(&resetIRButton);
    resetIRButton.setButtonText("Reset IR");
    resetIRButton.setColour(0x1000100, juce::Colours::whitesmoke.darker(1.0).withAlpha(1.0f));
    resetIRButton.setColour(0x1000c00, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    resetIRButton.setColour(0x1000101, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    resetIRButton.setColour(0x1000102, juce::Colours::black.brighter(0.1));
    resetIRButton.setColour(0x1000103, juce::Colours::black.brighter(0.1));
    
    setCabButtonProps();
    
    setSize (711, 500);
}

DiodeAmplifierAudioProcessorEditor::~DiodeAmplifierAudioProcessorEditor()
{
    inputSlider.setLookAndFeel(nullptr);
    driveSlider.setLookAndFeel(nullptr);
    lowSlider.setLookAndFeel(nullptr);
    midSlider.setLookAndFeel(nullptr);
    highSlider.setLookAndFeel(nullptr);
    outputSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void DiodeAmplifierAudioProcessorEditor::paint (juce::Graphics& g)
{
    //Image layer from Illustrator
    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::pluginBackground_png, BinaryData::pluginBackground_pngSize);
    g.drawImageWithin(pluginBackground, 0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::RectanglePlacement::stretchToFit);
        
    // Background dark-maker
    juce::Rectangle<float> backgroundDarker;
    backgroundDarker.setBounds(0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight());
    g.setColour(juce::Colour::fromFloatRGBA(0.09f, 0.10f, 0.12f, 0.65f));
    g.fillRect(backgroundDarker);

    // Header rectangle
    juce::Rectangle<float> header;
    header.setBounds(0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight() * 0.13f);
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillRect(header);
        
    // Set header text
    g.setFont (16.0f);
    g.setColour (juce::Colours::white.withAlpha(0.25f));
    g.drawFittedText ("Algorithms by Landon Viator", 12, AudioProcessorEditor::getHeight() * 0.05, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::topLeft, 1);
        
    // Logo
    mLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    g.drawImageWithin(mLogo, AudioProcessorEditor::getWidth() * 0.4f, AudioProcessorEditor::getHeight() * 0.025, AudioProcessorEditor::getHeight() * 0.08f * 4.58, AudioProcessorEditor::getHeight() * 0.08f, juce::RectanglePlacement::centred);
}

void DiodeAmplifierAudioProcessorEditor::resized()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();

    auto sliderSize {3.5};
    
    //first column of gui
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::row;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::center;

    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / sliderSize, bounds.getHeight() / sliderSize, inputSlider).withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / sliderSize, bounds.getHeight() / sliderSize, driveSlider).withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / sliderSize, bounds.getHeight() / sliderSize, outputSlider).withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));

    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromTop(bounds.getHeight() - 72));
    /* ============================================================================ */

    lowSlider.setBounds(inputSlider.getX(), inputSlider.getY() + inputSlider.getHeight() + 32, inputSlider.getWidth(), inputSlider.getHeight());
    midSlider.setBounds(driveSlider.getX(), driveSlider.getY() + driveSlider.getHeight() + 32, driveSlider.getWidth(), driveSlider.getHeight());
    highSlider.setBounds(outputSlider.getX(), outputSlider.getY() + outputSlider.getHeight() + 32, outputSlider.getWidth(), outputSlider.getHeight());
    
    brightButton.setBounds(outputSlider.getX() + outputSlider.getWidth() - 24, outputSlider.getY() * 1.5, 72, 32);
    cabButton.setBounds(brightButton.getX(), brightButton.getY() + brightButton.getHeight(), 72, 32);
    cabToggleButton.setBounds(cabButton.getX(), cabButton.getY() + cabButton.getHeight(), 72, 32);
    resetIRButton.setBounds(cabToggleButton.getX(), cabToggleButton.getY() + cabToggleButton.getHeight(), 72, 32);

    // Window border bounds
        windowBorder.setBounds
        (
            16,
            AudioProcessorEditor::getHeight() * 0.16,
            AudioProcessorEditor::getWidth() * 0.95,
            AudioProcessorEditor::getHeight() * .8
        );
    
}
