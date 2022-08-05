/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FirstPluginAudioProcessorEditor::FirstPluginAudioProcessorEditor (FirstPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setRange(-96.0, 48.0, 1.0);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    gainSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    gainSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    
    gainSlider.onValueChange = [this]()
    {
        audioProcessor.gain = gainSlider.getValue();
    };
    
    addAndMakeVisible(phaseInvertButton);
    phaseInvertButton.setClickingTogglesState(true);
    phaseInvertButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    phaseInvertButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow.darker(0.2f));
    phaseInvertButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black.brighter(0.1f));
    phaseInvertButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black.brighter(0.1f));
    
    phaseInvertButton.onClick = [this]()
    {
        
    };
    
    addAndMakeVisible(gainSliderLabel);
    gainSliderLabel.setText("Trim", juce::dontSendNotification);
    gainSliderLabel.setJustificationType(juce::Justification::centred);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    gainSliderLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    
    setSize (400, 400);
}

FirstPluginAudioProcessorEditor::~FirstPluginAudioProcessorEditor()
{
}

//==============================================================================
void FirstPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black.brighter(0.1f));
}

void FirstPluginAudioProcessorEditor::resized()
{
    float topMargin = getHeight() * 0.15;
    float leftMargin = getWidth() * 0.25;
    float dialSize = getWidth() * 0.5;
    
    gainSlider.setBounds(leftMargin, topMargin, dialSize, dialSize);
    
    phaseInvertButton.setBounds(gainSlider.getX() + gainSlider.getWidth() * 0.25, gainSlider.getY() + gainSlider.getHeight() + 16, gainSlider.getWidth() * 0.5, gainSlider.getWidth() * 0.25);
    
}
