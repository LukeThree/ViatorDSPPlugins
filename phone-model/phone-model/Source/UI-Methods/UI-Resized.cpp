/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void PhonemodelAudioProcessorEditor::uiResized(float width, float height)
{
    auto leftMargin = width * 0.1;
    auto topMargin = height * 0.69;
    auto dialSize = width * 0.13;
    
    // Plugin background UI
    windowComponent.setWidthAndHeight(width, height);
    windowComponent.setBounds(getLocalBounds());
    
    bandwithDial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    centerDial.setBounds(bandwithDial.getX() + bandwithDial.getWidth(), topMargin, dialSize, dialSize);
    resoDial.setBounds(centerDial.getX() + centerDial.getWidth(), topMargin, dialSize, dialSize);
    driveDial.setBounds(resoDial.getX() + resoDial.getWidth(), topMargin, dialSize, dialSize);
    lowShelfDial.setBounds(driveDial.getX() + driveDial.getWidth(), topMargin, dialSize, dialSize);
    highShelfDial.setBounds(lowShelfDial.getX() + lowShelfDial.getWidth(), topMargin, dialSize, dialSize);
    
    // Save plugin size in the tree
    saveWindowSize();
}
