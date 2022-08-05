/*
  ==============================================================================

    LV_PostToneComponent.cpp
    Created: 14 Jan 2022 7:11:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_PostToneComponent.h"

//==============================================================================
LV_PostToneComponent::LV_PostToneComponent(JustaFilterAudioProcessor& p) : audioProcessor(p)
{
    // Post stuff
    //addAndMakeVisible(postGroup);
    postGroup.setText("Post Tone");
    
    //addAndMakeVisible(moduleBypass);
    moduleBypass.setToggleStyle(juce::LV_Toggle::ToggleStyle::kPower);
    
    // Freq
    //addAndMakeVisible(freqDial);
    //addAndMakeVisible(freqDialLabel);
    freqDialLabel.attachToComponent(&freqDial, false);
    freqDialLabel.setText("Freq", juce::dontSendNotification);
    
    // Gain
    //addAndMakeVisible(gainDial);
    gainDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange.darker(0.5));
    gainDial.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    gainDial.forceShadow();
    //addAndMakeVisible(gainDialLabel);
    gainDialLabel.attachToComponent(&gainDial, false);
    gainDialLabel.setText("Gain", juce::dontSendNotification);
    
    // Q
    //addAndMakeVisible(qDial);
    qDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    qDial.forceShadow();
    //addAndMakeVisible(qDialLabel);
    qDialLabel.attachToComponent(&qDial, false);
    qDialLabel.setText("Width", juce::dontSendNotification);
}

LV_PostToneComponent::~LV_PostToneComponent()
{
    freqDialAttach = nullptr;
    gainDialAttach = nullptr;
    qDialAttach = nullptr;
    powerAttach = nullptr;
}

void LV_PostToneComponent::paint (juce::Graphics& g)
{
    
}

void LV_PostToneComponent::resized()
{
    auto leftMargin = getWidth() * 0.05;
    auto topMargin = getWidth() * 0.1;
    auto dialSize = width * 0.1;
    
    // Pre stuff
    postGroup.setBounds(0, 0, getWidth(), getHeight());
    moduleBypass.setBounds(preGroup.getX() + leftMargin / 3, preGroup.getY() + topMargin * 0.6, 48, 48);
    
    freqDial.setBounds(moduleBypass.getX() * 2.0 + dialSize * 1.25, moduleBypass.getY() * 8.0, dialSize, dialSize);
    gainDial.setBounds(moduleBypass.getX() * 2.0, freqDial.getY() + freqDial.getWidth(), dialSize, dialSize);
    qDial.setBounds(freqDial.getX(), gainDial.getY() + gainDial.getWidth(), dialSize, dialSize);

}

void LV_PostToneComponent::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
