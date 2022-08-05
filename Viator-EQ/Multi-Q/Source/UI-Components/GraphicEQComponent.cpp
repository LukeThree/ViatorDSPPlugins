/*
  ==============================================================================

    GraphicEQComponent.cpp
    Created: 12 Apr 2022 7:10:23pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GraphicEQComponent.h"

//==============================================================================
GraphicEQComponent::GraphicEQComponent(MultiQAudioProcessor& p) : audioProcessor(p)
, band1Label(true, "31")
, band2Label(true, "63")
, band3Label(true, "125")
, band4Label(true, "250")
, band5Label(true, "500")
, band6Label(true, "1K")
, band7Label(true, "2K")
, band8Label(true, "4K")
, band9Label(true, "8K")
, band10Label(true, "15K")
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    for (int i = 0; i < faders.size(); ++i)
    {
        addAndMakeVisible(faders[i]);
        addAndMakeVisible(labels[i]);
        labels[i]->attachToComponent(faders[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
    }
    
    band1FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter1GainID, band1);
    band2FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter2GainID, band2);
    band3FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter3GainID, band3);
    band4FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter4GainID, band4);
    band5FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter5GainID, band5);
    band6FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter6GainID, band6);
    band7FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter7GainID, band7);
    band8FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter8GainID, band8);
    band9FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter9GainID, band9);
    band10FaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, graphicFilter10GainID, band10);
}

GraphicEQComponent::~GraphicEQComponent()
{
    for (auto& fader : faders)
    {
        fader = nullptr;
    }
    
    faders.clear();
    faders.shrink_to_fit();
    
    for (auto& label : labels)
    {
        label = nullptr;
    }
    
    labels.clear();
    labels.shrink_to_fit();
    
    labelTexts.clear();
    labelTexts.shrink_to_fit();
    
    band1FaderAttach = nullptr;
    band2FaderAttach = nullptr;
    band3FaderAttach = nullptr;
    band4FaderAttach = nullptr;
    band5FaderAttach = nullptr;
    band6FaderAttach = nullptr;
    band7FaderAttach = nullptr;
    band8FaderAttach = nullptr;
    band9FaderAttach = nullptr;
    band10FaderAttach = nullptr;
}

void GraphicEQComponent::paint (juce::Graphics& g)
{
}

void GraphicEQComponent::resized()
{
    auto leftMargin = 12;
    auto topMargin = 24;
    auto faderWidth = getWidth() * 0.11;
    auto faderheight = faderWidth * 2.35;
    auto spaceBetween = 1.85;
    auto labelFont = faderWidth * 0.16f;

    band1.setBounds(leftMargin, topMargin, faderWidth, faderheight);
    band1Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));

    band2.setBounds(band1.getX() + band1.getWidth() * spaceBetween, topMargin, faderWidth, faderheight);
    band2Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band3.setBounds(band2.getX() + band2.getWidth() * spaceBetween, topMargin, faderWidth, faderheight);
    band3Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band4.setBounds(band3.getX() + band3.getWidth() * spaceBetween, topMargin, faderWidth, faderheight);
    band4Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band5.setBounds(band4.getX() + band4.getWidth() * spaceBetween, topMargin, faderWidth, faderheight);
    band5Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    auto bottomMargin = band4.getY() + band4.getHeight() * 1.2;
    
    band6.setBounds(band1.getX(), bottomMargin, faderWidth, faderheight);
    band6Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band7.setBounds(band2.getX(), bottomMargin, faderWidth, faderheight);
    band7Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band8.setBounds(band3.getX(), bottomMargin, faderWidth, faderheight);
    band8Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band9.setBounds(band4.getX(), bottomMargin, faderWidth, faderheight);
    band9Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    band10.setBounds(band5.getX(), bottomMargin, faderWidth, faderheight);
    band10Label.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
}

void GraphicEQComponent::reset(bool reset)
{
    if (reset)
    {
        for (auto& fader : faders)
        {
            fader->setValue(0.0);
        }
    }
}
