/*
  ==============================================================================

    LV_DialComponent.cpp
    Created: 24 Oct 2021 12:17:36am
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_DialComponent.h"

//==============================================================================
LV_DialComponent::LV_DialComponent(juce::String suffix, double rangeStart, double rangeEnd, double intervalValue, double returnValue, int dialStyle)
{
    init_shadows();
    dial.setRange(rangeStart, rangeEnd, intervalValue);
    dial.setDoubleClickReturnValue(true, returnValue);
    dial.setTextValueSuffix(suffix);
    setDialStyle(dialStyle);
    addAndMakeVisible(dial);
}

LV_DialComponent::~LV_DialComponent()
{
    dial.setLookAndFeel(nullptr);
}

void LV_DialComponent::paint (juce::Graphics& g)
{
}

void LV_DialComponent::resized()
{
    dial.setBounds(getLocalBounds());
}

void LV_DialComponent::setColour(int colourID, juce::Colour newColour)
{
    dial.setColour(colourID, newColour);
}

void LV_DialComponent::init_shadows()
{
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (-1, 4);
    shadowProperties.colour = juce::Colours::black.withAlpha(0.5f);
    dialShadow.setShadowProperties (shadowProperties);
}

void LV_DialComponent::enableShadow(bool enable)
{
    dial.setComponentEffect(nullptr);
    
    if (enable)
    {
        dial.setComponentEffect(&dialShadow);
    }
}

void LV_DialComponent::setDialStyle(int dialStyle)
{
    jassert(dialStyle < 4 && dialStyle > 0);
    
    switch (dialStyle)
    {
        case 1:
            dial.setLookAndFeel(&hardDial);
            break;
            
        case 2:
            dial.setLookAndFeel(&alphaDial);
            break;
            
        case 3:
            dial.setLookAndFeel(&ableDial);
            break;
    }
}
