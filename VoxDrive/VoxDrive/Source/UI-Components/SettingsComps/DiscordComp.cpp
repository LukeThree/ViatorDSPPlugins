/*
  ==============================================================================

    DiscordComp.cpp
    Created: 30 May 2022 7:04:17pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DiscordComp.h"

//==============================================================================
DiscordComp::DiscordComp()
{
}

DiscordComp::~DiscordComp()
{
}

void DiscordComp::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black.brighter(0.15f));
    g.fillRoundedRectangle(getWidth() * 0.05, getHeight() * 0.05, getWidth() * 0.9, getHeight() * 0.9, 2.0f);
    
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.drawRoundedRectangle(getWidth() * 0.05, getHeight() * 0.05, getWidth() * 0.9, getHeight() * 0.9, 2.0f, 2.0f);
    
    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(getLocalBounds());
    
    g.setColour(juce::Colours::dimgrey);
    g.setFont(juce::Font ("Helvetica", getHeight() * 0.45f, juce::Font::FontStyleFlags::bold));
    g.drawFittedText("Click to join my Discord community!", getLocalBounds(), juce::Justification::centred, 1);
}

void DiscordComp::resized()
{
}
