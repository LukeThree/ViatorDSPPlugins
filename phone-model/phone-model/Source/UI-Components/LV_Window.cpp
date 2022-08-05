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
    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::naturalwood_jpeg, BinaryData::naturalwood_jpegSize);
    
    // Draw and position the image
    g.drawImageWithin(pluginBackground, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    
    // UI Background
    g.setColour(juce::Colour::fromFloatRGBA(0.09f, 0.10f, 0.12f, 0.75f));
    juce::Rectangle<float> mainBackground;
    mainBackground.setBounds(getWidth() * 0.05f, getHeight() * 0.6f, getWidth() * 0.9f, getHeight() * 0.37f);
    g.drawRoundedRectangle(mainBackground, 8.0, 4.0);
    
    // Light accent
    g.setColour(juce::Colours::whitesmoke.darker(0.1f).withAlpha(0.25f));
    juce::Rectangle<float> lightAccent;
    lightAccent.setBounds(mainBackground.getX() + 2, mainBackground.getY() + 2, mainBackground.getWidth() - 4, mainBackground.getHeight() - 4);
    g.drawRoundedRectangle(lightAccent, 8.0, 2.0);

    // Light accent
    juce::Rectangle<float> innerBackground;
    innerBackground.setBounds(lightAccent.getX() + 1, lightAccent.getY() + 1, lightAccent.getWidth() - 2, lightAccent.getHeight() - 2);
    g.setColour(juce::Colours::black.brighter(0.1f).withAlpha(0.9f));
    g.fillRoundedRectangle(innerBackground, 4.0);
}

void LV_Window::paintLogoAndLink(juce::Graphics &g)
{
    // Logo layer
    footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    
    if(!paintRan)
    {
        footerLogo.multiplyAllAlphas(0.25f);
        paintRan = true;
    }
    
    // Draw and position the image
    g.drawImageWithin(footerLogo, width * 0.8, 1, width * 0.2, height * 0.1, juce::RectanglePlacement::centred);
    
    //Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(width * 0.8, 1, width * 0.2, height * 0.1);
}

void LV_Window::paintText(juce::Graphics &g)
{
    // Plugin text
    g.setColour (juce::Colours::whitesmoke.withAlpha(0.125f));
    g.setFont(juce::Font ("Helvetica", width * 0.02, juce::Font::FontStyleFlags::plain));
    g.drawFittedText ("TeleVox v1.0.0", width * 0.01, 25, width, height, juce::Justification::topLeft, 1);
}

void LV_Window::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
