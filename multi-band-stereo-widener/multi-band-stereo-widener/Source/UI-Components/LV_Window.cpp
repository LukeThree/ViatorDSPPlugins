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
//    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::testTexture_png, BinaryData::testTexture_pngSize);
    
    // Draw and position the image
//    g.drawImageWithin(pluginBackground, 0, 0, width, height, juce::RectanglePlacement::stretchToFit);
    g.fillAll(juce::Colours::black.brighter(0.1));
    
    g.setColour(juce::Colours::slategrey.darker(0.25));
    g.fillRect(500, 250, 250, 250);
}

void LV_Window::paintLogoAndLink(juce::Graphics &g)
{
    // Logo layer
    footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    footerLogo.multiplyAllAlphas(0.25f);
    
    // Draw and position the image
    g.drawImageWithin(footerLogo, width * 0.38, height * 0.9, width * 0.25, height * 0.1, juce::RectanglePlacement::centred);
    
    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(width * 0.38, height * 0.9, width * 0.25, height * 0.1);
}

void LV_Window::paintText(juce::Graphics &g)
{
    // Plugin text
    g.setColour (juce::Colours::whitesmoke.withAlpha(0.125f));
    g.setFont(juce::Font ("Helvetica", width * 0.015, juce::Font::FontStyleFlags::plain));
    g.drawFittedText ("Plugin v1.0.0", width * 0.01, height * 0.02, width, height, juce::Justification::topLeft, 1);
}

void LV_Window::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
