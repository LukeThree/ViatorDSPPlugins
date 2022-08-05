/*
  ==============================================================================

    CompressorDials.cpp
    Created: 20 Jun 2021 4:05:56pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setCompressorDials()
{
    threshDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, threshId, threshDial);
    threshDial.setRange(-25.0, 20.0, 0.5);
    threshDial.setSkewFactorFromMidPoint(-2.0);
    threshDial.setTextValueSuffix(" dB");
    threshDial.setDoubleClickReturnValue(true, -2.0);
    
    ratioDial.setTextBoxIsEditable(false);
    ratioDial.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::white.withAlpha(0.0f));
    ratioDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, ratioId, ratioDial);
    ratioDial.setRange(0.0, 5.0, 1.0);
    ratioDial.setTextValueSuffix(":1");
    ratioDial.setDoubleClickReturnValue(true, 0.0);
    
    addAndMakeVisible(ratioValueLabel);
    ratioValueLabel.setJustificationType(juce::Justification::centredTop);
    ratioValueLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    ratioValueLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    ratioValueLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    setRatioLabelValue();
    
    addAndMakeVisible(speedValueLabel);
    speedValueLabel.setJustificationType(juce::Justification::centredTop);
    speedValueLabel.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    speedValueLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25));
    speedValueLabel.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    speedValueLabel.setText(audioProcessor.getSpeedDialStrings(), juce::dontSendNotification);
    DBG("Label set!");
    speedValueLabel.setBounds(speedDial.getX(), speedDial.getY() + speedDial.getHeight() - 24, ratioValueLabel.getWidth(), ratioValueLabel.getHeight());
    
    compGainDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, makeUpId, compGainDial);
    compGainDial.setRange(-6.0, 20.0, 0.5);
    compGainDial.setSkewFactorFromMidPoint(6.0);
    compGainDial.setDoubleClickReturnValue(true, 6.0);
    
    speedDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, speedId, speedDial);
    speedDial.setTextBoxIsEditable(false);
    speedDial.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::white.withAlpha(0.0f));
    speedDial.setRange(0.0, 5.0, 1.0);
    speedDial.setDoubleClickReturnValue(true, 0.0);
    
    blendDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, blendId, blendDial);
    blendDial.setRange(0.0, 100.0, 1.0);
    blendDial.setTextValueSuffix(" %");
    blendDial.setDoubleClickReturnValue(true, 0.0);
}

void BedroomCompAudioProcessorEditor::setRatioLabelValue()
{
    /* Round the ratio to one decimal */
    auto labelValue = float_one_point_round(audioProcessor.getRatioForDial());
    
    /* Convert to string for label */
    auto labelString = std::to_string(labelValue);
    
    /* If ratio is 1.5, remove excess zeros, else, remove .0 to disply as int */

    if (audioProcessor.getRatioForDial() < 2.0)
    {
        labelString.erase(std::remove(labelString.begin(), labelString.end(), '0'), labelString.end());
    }

    else
    {
        labelString.erase(std::remove(labelString.begin(), labelString.end(), '0'), labelString.end());
        labelString.erase(std::remove(labelString.begin(), labelString.end(), '.'), labelString.end());
    }
    
    /* set the label text */
    ratioValueLabel.setText(labelString + ":1", juce::dontSendNotification);
}
