/*
  ==============================================================================

    ParametricEQComponent.cpp
    Created: 12 Apr 2022 7:10:38pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParametricEQComponent.h"

//==============================================================================
ParametricEQComponent::ParametricEQComponent(MultiQAudioProcessor& p) : audioProcessor(p)
, band1Label(true, "Low Gain")
, band2Label(true, "Mid Gain")
, band3Label(true, "Mid Gain")
, band4Label(true, "high Gain")
, band1FreqLabel(true, "Low Freq")
, band2FreqLabel(true, "Mid Freq")
, band3FreqLabel(true, "Mid Freq")
, band4FreqLabel(true, "high Freq")
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    for (int i = 0; i < dials.size(); i++)
    {
        addAndMakeVisible(dials[i]);
        addAndMakeVisible(labels[i]);
        labels[i]->attachToComponent(dials[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
    }
    
    band1GainDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter1GainID, band1GainDial);
    band2GainDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter2GainID, band2GainDial);
    band3GainDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter3GainID, band3GainDial);
    band4GainDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter4GainID, band4GainDial);
    band1FreqDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter1FreqID, band1FreqDial);
    band2FreqDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter2FreqID, band2FreqDial);
    band3FreqDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter3FreqID, band3FreqDial);
    band4FreqDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, parametricFilter4FreqID, band4FreqDial);
    
    band1FreqDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    band1FreqDial.forceShadow();
    band2FreqDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    band2FreqDial.forceShadow();
    band3FreqDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    band3FreqDial.forceShadow();
    band4FreqDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    band4FreqDial.forceShadow();
}

ParametricEQComponent::~ParametricEQComponent()
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
    
    band1GainDialAttach = nullptr;
    band2GainDialAttach = nullptr;
    band3GainDialAttach = nullptr;
    band4GainDialAttach = nullptr;
    
    band1FreqDialAttach = nullptr;
    band2FreqDialAttach = nullptr;
    band3FreqDialAttach = nullptr;
    band4FreqDialAttach = nullptr;
    
}

void ParametricEQComponent::paint (juce::Graphics& g)
{
}

void ParametricEQComponent::resized()
{
    auto leftMargin = 0;
    auto topMargin = 24;
    auto sliderSize = getWidth() * 0.24;
    auto labelSize = sliderSize * 0.08f;
    
    band1GainDial.setBounds(leftMargin, topMargin, sliderSize, sliderSize);
    band1Label.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band1FreqDial.setBounds(band1GainDial.getX(), band1GainDial.getY() + band1GainDial.getHeight() * 1.25, sliderSize, sliderSize);
    band2Label.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band2GainDial.setBounds(band1GainDial.getX() + band1GainDial.getWidth(), topMargin, sliderSize, sliderSize);
    band3Label.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band2FreqDial.setBounds(band2GainDial.getX(), band2GainDial.getY() + band2GainDial.getHeight() * 1.25, sliderSize, sliderSize);
    band4Label.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band3GainDial.setBounds(band2GainDial.getX() + band2GainDial.getWidth(), topMargin, sliderSize, sliderSize);
    band1FreqLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band3FreqDial.setBounds(band3GainDial.getX(), band3GainDial.getY() + band3GainDial.getHeight() * 1.25, sliderSize, sliderSize);
    band2FreqLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band4GainDial.setBounds(band3GainDial.getX() + band3GainDial.getWidth(), topMargin, sliderSize, sliderSize);
    band3FreqLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));

    band4FreqDial.setBounds(band4GainDial.getX(), band4GainDial.getY() + band4GainDial.getHeight() * 1.25, sliderSize, sliderSize);
    band4FreqLabel.setFont(juce::Font ("Helvetica", labelSize, juce::Font::FontStyleFlags::bold));
}

void ParametricEQComponent::reset(bool reset)
{
    if (reset)
    {
        band1GainDial.setValue(0.0);
        band2GainDial.setValue(0.0);
        band3GainDial.setValue(0.0);
        band4GainDial.setValue(0.0);
        band1FreqDial.setValue(200.0);
        band2FreqDial.setValue(1000.0);
        band3FreqDial.setValue(1000.0);
        band4FreqDial.setValue(7000.0);
    }
}
