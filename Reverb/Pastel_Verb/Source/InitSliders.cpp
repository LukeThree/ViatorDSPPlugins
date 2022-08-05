/*
  ==============================================================================

    InitSliders.cpp
    Created: 20 Aug 2021 9:48:15pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::initSliders()
{
    for (auto i {0}; i < sliders.size(); i++)
    {
        // Making visible
        addAndMakeVisible(sliders[i]);
        
        //  All sliders except the last two (faders)
        if (i < sliders.size() - 2)
        {
            sliders[i]->setLookAndFeel(&customDial);
            sliders[i]->setComponentEffect(&dialShadow);
        }
        
        // Last two are faders
        else
        {
            sliders[i]->setLookAndFeel(&customFader);
            sliders[i]->setComponentEffect(&sliderShadow);
        }
    }
}
