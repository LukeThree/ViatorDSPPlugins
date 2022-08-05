/*
  ==============================================================================

    TubeEQComponent.cpp
    Created: 12 Apr 2022 7:10:54pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TubeEQComponent.h"

//==============================================================================
TubeEQComponent::TubeEQComponent(MultiQAudioProcessor& p) : audioProcessor(p)
, lowBoostLabel(true, "Low Boost")
, lowCutLabel(true, "Low Cut")
, lowFreqLabel(true, "Low Freq")
, bandwidthLabel(true, "BW")
, highBoostLabel(true, "High Boost")
, highCutLabel(true, "High Cut")
, highFreqLabel(true, "High Freq")
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    for (int i = 0; i < dials.size(); i++)
    {
        addAndMakeVisible(dials[i]);
        addAndMakeVisible(labels[i]);
        labels[i]->attachToComponent(dials[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
    }
    
    lowBoostAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeLowBoostID, lowBoostDial);
    lowCutAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeLowCutID, lowCutDial);
    lowFreqAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeLowFreqID, lowFreqDial);
    bandwidthAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeFilterBWID, bandwidthDial);
    highBoostAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeHighBoostID, highBoostDial);
    highCutAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeHighCutID, highCutDial);
    highFreqAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, tubeHighFreqID, highFreqDial);
    
    lowFreqDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    lowFreqDial.forceShadow();
    highFreqDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    highFreqDial.forceShadow();
    bandwidthDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange.darker(0.5));
    bandwidthDial.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    bandwidthDial.forceShadow();
}

TubeEQComponent::~TubeEQComponent()
{
    for (auto& dial : dials)
    {
        dial = nullptr;
    }
    
    dials.clear();
    dials.shrink_to_fit();
    
    for (auto& label : labels)
    {
        label = nullptr;
    }
    
    labels.clear();
    labels.shrink_to_fit();
    
    labelTexts.clear();
    labelTexts.shrink_to_fit();
    
    lowBoostAttach = nullptr;
    lowCutAttach = nullptr;
    lowFreqAttach = nullptr;
    bandwidthAttach = nullptr;
    highBoostAttach = nullptr;
    highCutAttach = nullptr;
    highFreqAttach = nullptr;
    
}

void TubeEQComponent::paint (juce::Graphics& g)
{
}

void TubeEQComponent::resized()
{
    auto leftMargin = 0;
    auto topMargin = 36;
    auto dialSize = getHeight() * 0.38;
    auto smallDialSize = dialSize * 0.85;
    auto spaceBetweenDials = 1.05;
    auto labelSize = dialSize * 0.08f;
    
    lowBoostDial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    lowBoostLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    lowCutDial.setBounds(lowBoostDial.getX() + lowBoostDial.getWidth() * spaceBetweenDials, topMargin, dialSize, dialSize);
    lowCutLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    highBoostDial.setBounds(lowCutDial.getX() + lowCutDial.getWidth() * spaceBetweenDials, topMargin, dialSize, dialSize);
    highBoostLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    highCutDial.setBounds(highBoostDial.getX() + highBoostDial.getWidth() * spaceBetweenDials, topMargin, dialSize, dialSize);
    highCutLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));
        
    lowFreqDial.setBounds(lowBoostDial.getX() + (lowBoostDial.getWidth() / 1.55), lowBoostDial.getY() + lowBoostDial.getHeight() * 1.2, smallDialSize, smallDialSize);
    lowFreqLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    bandwidthDial.setBounds(lowFreqDial.getX() + lowFreqDial.getWidth() * 1.3, lowFreqDial.getY(), smallDialSize, smallDialSize);
    bandwidthLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    highFreqDial.setBounds(highBoostDial.getX() + (highBoostDial.getWidth() / 1.55), bandwidthDial.getY(), smallDialSize, smallDialSize);
    highFreqLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));
}

void TubeEQComponent::reset(bool reset)
{
    if (reset)
    {
        lowBoostDial.setValue(0.0);
        lowCutDial.setValue(0.0);
        lowFreqDial.setValue(100.0);
        highBoostDial.setValue(0.0);
        highCutDial.setValue(0.0);
        highFreqDial.setValue(5000.0);
        bandwidthDial.setValue(5.0);
    }
}
