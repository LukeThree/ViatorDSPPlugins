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
LV_HeaderComponent::LV_HeaderComponent()
{
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
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
    mWebLink.setBounds(width * 0.025, height * 0.01, width * 0.125, height * 0.065);
}

void LV_HeaderComponent::resized()
{
}

void LV_HeaderComponent::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
