/*
  ==============================================================================

    LV_FilterComponent.cpp
    Created: 3 Feb 2022 9:02:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_FilterComponent.h"

//==============================================================================
LV_FilterComponent::LV_FilterComponent(JustaFilterAudioProcessor& p) : audioProcessor(p)
{
    // Border
    addAndMakeVisible(filterBorder);
    filterBorder.setText("Just a Filter");
    
    // Cutoff Dial
    cutoffDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, cutoffID, cutoffDial);
    addAndMakeVisible(cutoffDial);
    
    // Q Dial
    qDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, bandwidthID, qDial);
    addAndMakeVisible(qDial);
    qDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange.darker(0.5));
    qDial.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    qDial.forceShadow();
    
    // Gain Dial
    gainDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, gainID, gainDial);
    addAndMakeVisible(gainDial);
    gainDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    gainDial.forceShadow();
    
    // Phase toggle
    phaseAttach = std::make_unique<ButtonAttachment>(audioProcessor.treeState, phaseID, phaseToggle);
    addAndMakeVisible(phaseToggle);
    phaseToggle.setToggleStyle(viator_gui::Toggle::ToggleStyle::kPhase);
}

LV_FilterComponent::~LV_FilterComponent()
{
    cutoffDialAttach = nullptr;
    qDialAttach = nullptr;
    gainDialAttach = nullptr;
    phaseAttach = nullptr;
}

void LV_FilterComponent::paint (juce::Graphics& g)
{

}

void LV_FilterComponent::resized()
{
    auto leftMargin = getWidth() * 0.05f;
    auto topMargin = getHeight() * 0.05f;
    auto dialSize = getWidth() * 0.25f;
    auto spaceBetween = 1.1;
    auto toggleSize = getWidth() * 0.05;
    
    filterBorder.setBounds(leftMargin, topMargin, getWidth() - leftMargin * 2.0f, getHeight() - topMargin * 2.0f);
    phaseToggle.setBounds(leftMargin * 1.125, topMargin * 1.5, toggleSize, toggleSize);
    
    cutoffDial.setBounds(leftMargin * 2.0, topMargin * 3.5f, dialSize, dialSize);
    qDial.setBounds(cutoffDial.getX() + cutoffDial.getWidth() * spaceBetween, cutoffDial.getY() * 1.75f, dialSize, dialSize);
    gainDial.setBounds(qDial.getX() + qDial.getWidth() * spaceBetween, cutoffDial.getY(), dialSize, dialSize);
}
