/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void PhonemodelAudioProcessorEditor::uiConstructor()
{
    addAndMakeVisible(windowComponent);
    
    // Window
    initWindow();
    
    addAndMakeVisible(bandwithDial);
    bandwithDial.setDialStyle(LV_DialComponent::DialStyle::kHardDial);
    bandwithDial.enableShadow(true);
    addAndMakeVisible(bandwidthLabel);
    bandwidthLabel.attachToComponent(&bandwithDial, false);
    bandwidthLabel.setJustificationType(juce::Justification::centred);
    bandwidthLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.831, 0.765, 0.663, 0.8));
    
    addAndMakeVisible(centerDial);
    centerDial.setDialStyle(LV_DialComponent::DialStyle::kHardDial);
    centerDial.enableShadow(true);
    centerDial.setDialSkew(800.0f);
    addAndMakeVisible(centerLabel);
    centerLabel.attachToComponent(&centerDial, false);
    centerLabel.setJustificationType(juce::Justification::centred);
    centerLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.831, 0.765, 0.663, 0.8));
    
    addAndMakeVisible(driveDial);
    driveDial.setDialStyle(LV_DialComponent::DialStyle::kHardDial);
    driveDial.enableShadow(true);
    addAndMakeVisible(driveLabel);
    driveLabel.attachToComponent(&driveDial, false);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.831, 0.765, 0.663, 0.8));
    
    addAndMakeVisible(resoDial);
    resoDial.setDialStyle(LV_DialComponent::DialStyle::kHardDial);
    resoDial.enableShadow(true);
    addAndMakeVisible(resoLabel);
    resoLabel.attachToComponent(&resoDial, false);
    resoLabel.setJustificationType(juce::Justification::centred);
    resoLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.831, 0.765, 0.663, 0.8));
    
    addAndMakeVisible(lowShelfDial);
    lowShelfDial.setDialStyle(LV_DialComponent::DialStyle::kHardDial);
    lowShelfDial.enableShadow(true);
    addAndMakeVisible(lowShelfDialLabel);
    lowShelfDialLabel.attachToComponent(&lowShelfDial, false);
    lowShelfDialLabel.setJustificationType(juce::Justification::centred);
    lowShelfDialLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.831, 0.765, 0.663, 0.8));
    
    addAndMakeVisible(highShelfDial);
    highShelfDial.setDialStyle(LV_DialComponent::DialStyle::kHardDial);
    highShelfDial.enableShadow(true);
    addAndMakeVisible(highShelfDialLabel);
    highShelfDialLabel.attachToComponent(&highShelfDial, false);
    highShelfDialLabel.setJustificationType(juce::Justification::centred);
    highShelfDialLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.831, 0.765, 0.663, 0.8));
}
