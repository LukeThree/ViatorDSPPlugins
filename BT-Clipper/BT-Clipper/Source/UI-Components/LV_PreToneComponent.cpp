/*
  ==============================================================================

    LV_PreToneComponent.cpp
    Created: 14 Jan 2022 7:01:32pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_PreToneComponent.h"

//==============================================================================
LV_PreToneComponent::LV_PreToneComponent(BTClipperAudioProcessor& p) : audioProcessor(p)
{
    // Pre stuff
    addAndMakeVisible(preGroup);
    preGroup.setText("Pre Tone");
    
    powerAttach = std::make_unique<ButtonAttachment>(audioProcessor.treeState, prePowerID, moduleBypass);
    addAndMakeVisible(moduleBypass);
    moduleBypass.setToggleStyle(juce::LV_Toggle::ToggleStyle::kPower);
    
    // Freq
    freqDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, preFreqID, freqDial);
    addAndMakeVisible(freqDial);
    addAndMakeVisible(freqDialLabel);
    freqDialLabel.attachToComponent(&freqDial, false);
    freqDialLabel.setText("Freq", juce::dontSendNotification);
    
    // Gain
    gainDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, preGainID, gainDial);
    addAndMakeVisible(gainDial);
    gainDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange.darker(0.5));
    gainDial.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    gainDial.forceShadow();
    addAndMakeVisible(gainDialLabel);
    gainDialLabel.attachToComponent(&gainDial, false);
    gainDialLabel.setText("Gain", juce::dontSendNotification);
    
    // Q
    qDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, preQID, qDial);
    addAndMakeVisible(qDial);
    qDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    qDial.forceShadow();
    addAndMakeVisible(qDialLabel);
    qDialLabel.attachToComponent(&qDial, false);
    qDialLabel.setText("Width", juce::dontSendNotification);
}

LV_PreToneComponent::~LV_PreToneComponent()
{
    freqDialAttach = nullptr;
    gainDialAttach = nullptr;
    qDialAttach = nullptr;
    powerAttach = nullptr;
}

void LV_PreToneComponent::paint (juce::Graphics& g)
{
}

void LV_PreToneComponent::resized()
{
    auto leftMargin = getWidth() * 0.05;
    auto topMargin = getWidth() * 0.1;
    auto dialSize = width * 0.1;
    
    // Pre stuff
    preGroup.setBounds(0, 0, getWidth(), getHeight());
    moduleBypass.setBounds(preGroup.getX() + leftMargin / 3, preGroup.getY() + topMargin * 0.6, 48, 48);
    
    freqDial.setBounds(moduleBypass.getX() * 2.0, moduleBypass.getY() * 8.0, dialSize, dialSize);
    gainDial.setBounds(freqDial.getX() + freqDial.getWidth() * 1.25, freqDial.getY() + freqDial.getWidth(), dialSize, dialSize);
    qDial.setBounds(freqDial.getX(), gainDial.getY() + gainDial.getWidth(), dialSize, dialSize);

}

void LV_PreToneComponent::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
