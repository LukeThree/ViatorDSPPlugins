/*
  ==============================================================================

    LV_Window.cpp
    Created: 23 Oct 2021 12:53:38am
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_Window.h"

//==============================================================================
LV_Window::LV_Window()
{
}

LV_Window::~LV_Window()
{
}

void LV_Window::paint (juce::Graphics& g)
{
    // Paint UI components
    paintBackground(g);
    paintLogoAndLink(g);
    paintText(g);
}

void LV_Window::resized()
{
}

void LV_Window::paintBackground(juce::Graphics &g)
{
    //Image layer from Illustrator
    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::pluginBackground2_png, BinaryData::pluginBackground2_pngSize);
    
    // Draw and position the image
    g.drawImageWithin(pluginBackground, 0, 0, width, height, juce::RectanglePlacement::stretchToFit);
}

void LV_Window::paintLogoAndLink(juce::Graphics &g)
{
    // Logo layer
    footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    
    // Draw and position the image
    g.drawImageWithin(footerLogo, width * 0.38, height * 0.8 + 4, width * 0.25, height * 0.1, juce::RectanglePlacement::centred);
    
    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(width * 0.38, height * 0.8 + 4, width * 0.25, height * 0.1);
}

void LV_Window::paintText(juce::Graphics &g)
{
    // Plugin text
    g.setColour (juce::Colours::whitesmoke.withAlpha(0.125f));
    g.setFont(width * 0.015);
    g.drawFittedText ("LV-50A v2.1.1", width * 0.12, height * 0.84, width, height, juce::Justification::topLeft, 1);
}

void LV_Window::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
