/*
  ==============================================================================

    LabelProps.cpp
    Created: 20 Jun 2021 8:53:29pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setCommonLabelProps(juce::Label &label)
{
    addAndMakeVisible(label);
    label.setJustificationType(juce::Justification::centredTop);
    label.setColour(0x1000280, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
    label.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5));
    label.setColour(0x1000282, juce::Colour::fromFloatRGBA(0, 0, 0, 0));

    inputDialLabel.setText("Gain", juce::dontSendNotification);
    hpfDialLabel.setText("HPF", juce::dontSendNotification);
    saturationDialLabel.setText("Saturation", juce::dontSendNotification);
    
    lowGainDialLabel.setText("LS", juce::dontSendNotification);
    midGainDialLabel.setText("Mid", juce::dontSendNotification);
    highGainDialLabel.setText("HS", juce::dontSendNotification);
    lowFreqDialLabel.setText("Low Freq", juce::dontSendNotification);
    midFreqDialLabel.setText("Mid Freq", juce::dontSendNotification);
    
    threshDialLabel.setText("Thresh", juce::dontSendNotification);
    ratioDialLabel.setText("Ratio", juce::dontSendNotification);
    speedDialLabel.setText("Speed", juce::dontSendNotification);
    compGainDialLabel.setText("Make-Up", juce::dontSendNotification);
    blendDialLabel.setText("Blend", juce::dontSendNotification);

}
