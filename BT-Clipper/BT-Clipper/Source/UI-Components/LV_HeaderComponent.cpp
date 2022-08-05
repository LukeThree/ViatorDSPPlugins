/*
  ==============================================================================

    LV_HeaderComponent.cpp
    Created: 17 Jan 2022 5:31:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_HeaderComponent.h"

//==============================================================================
LV_HeaderComponent::LV_HeaderComponent(juce::AudioProcessorValueTreeState& tree)
{
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    oversamplingMenu.addItem("No OS", 1);
    oversamplingMenu.addItem("8X OS", 2);
    oversamplingMenuAttach = std::make_unique<ComboBoxAttachment>(tree, qualityID, oversamplingMenu);
    addAndMakeVisible(oversamplingMenu);
    
    clipTypeMenu.addItem("Hard Cip", 1);
    clipTypeMenu.addItem("Soft Clip", 2);
    clipTypeMenu.addItem("Analog Clip", 3);
    clipTypeMenuAttach = std::make_unique<ComboBoxAttachment>(tree, clipTypeID, clipTypeMenu);
    addAndMakeVisible(clipTypeMenu);
}

LV_HeaderComponent::~LV_HeaderComponent()
{
}

void LV_HeaderComponent::paint (juce::Graphics& g)
{
    // Header rectangle
    juce::Rectangle<float> header;
    header.setBounds(0, 0, width, height);
    g.setColour(juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    g.fillRect(header);

    // Logo layer
    footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);

    // Draw and position the image
    g.drawImageWithin(footerLogo, width * 0.025, height * 0.01, width * 0.125, height * 0.065, juce::RectanglePlacement::centred);

    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(width * 0.38, height * 0.9, width * 0.25, height * 0.1);
}

void LV_HeaderComponent::resized()
{
    float rightMargin = width * 0.99f;
    float spaceBetween = width * 0.01f;
    float topMargin = height * 0.0125f;
    float menuWidth = width * 0.15f;
    float menuHeight = height * 0.06f;
    
    oversamplingMenu.setBounds(rightMargin - menuWidth, topMargin, menuWidth, menuHeight);
    clipTypeMenu.setBounds(oversamplingMenu.getX() - spaceBetween - oversamplingMenu.getWidth(), topMargin, menuWidth, menuHeight);
}

void LV_HeaderComponent::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
