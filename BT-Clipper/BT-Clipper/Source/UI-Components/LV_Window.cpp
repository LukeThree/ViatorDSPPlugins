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
    
    background = juce::ImageCache::getFromMemory(BinaryData::dark_blue_png, BinaryData::dark_blue_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    
//    juce::Rectangle<int> background = getBounds();
//    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0), height * .25, juce::Colour::fromFloatRGBA(0.12f, 0.14f, 0.18f, 1.0), height * .99));
//    g.fillRect(background);
}

void LV_Window::resized()
{
}

void LV_Window::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
