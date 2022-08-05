/*
  ==============================================================================

    UI_Controls.cpp
    Created: 9 Nov 2021 4:42:02pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UI_Controls.h"
#include "../PluginProcessor.h"

//==============================================================================
UI_Controls::UI_Controls(juce::AudioProcessorValueTreeState& tree) :
inputDial (" dB", -6.0, 12.0, 0.01, 0.0, tree, inputID),
driveDial (" dB", -50.0, 50.0, 0.01, 0.0, tree, driveID),
crossOverDial (" Hz", 20.0, 20000.0, 1.0, 2500.0, tree, crossOverID),
mixDial (" %", 0.0, 100.0, 1.0, 0.0, tree, mixID),
outputDial (" dB", -24.0, 24.0, 0.25, 0.0, tree, outputID)
{
    for (auto dial {0}; dial < dials.size(); dial++)
    {
        addAndMakeVisible(dials[dial]);
        dials[dial]->setDialStyle(LV_DialComponent::DialStyle::kHardDial);
        dials[dial]->enableShadow(true);
    }
    
    crossOverDial.setMidSkew(1000.0);
    
    for (auto label {0}; label < labels.size(); label++)
    {
        addAndMakeVisible(labels[label]);
        labels[label]->attachToComponent(dials[label], false);
        labels[label]->setJustificationType(juce::Justification::centred);
        labels[label]->setFont(juce::Font ("Helvetica", 12.0f, juce::Font::FontStyleFlags::bold));
        labels[label]->setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 1.0f).darker(0.5f));
    }
    
    bandMenu.addItem("All Bands", 1);
    bandMenu.addItem("Low Band", 2);
    bandMenu.addItem("High Band", 3);
    bandMenuAttach = std::make_unique<ComboBoxAttachment>(tree, bandChoiceID, bandMenu);
    addAndMakeVisible(bandMenu);
}

UI_Controls::~UI_Controls()
{
    bandMenuAttach = nullptr;
}

void UI_Controls::paint (juce::Graphics& g)
{
}

void UI_Controls::resized()
{
    auto leftMargin = width * 0.05;
    auto topMargin = height * 0.3;
    auto dialSize = width * 0.15;
    auto spaceBetween = 1.25;
    
    for (auto dial {0}; dial < dials.size(); dial++)
    {
        if (dials[dial] == &inputDial)
        {
            dials[dial]->setBounds(leftMargin, topMargin, dialSize, dialSize);
        }
        
        else
        {
            dials[dial]->setBounds(dials[dial - 1]->getX() + dials[dial - 1]->getWidth() * spaceBetween, topMargin, dialSize, dialSize);
        }
    }
    
    bandMenu.setBounds(crossOverDial.getX(), crossOverDial.getY() - dialSize, dialSize, dialSize / 2.0);
}

void UI_Controls::setBoundsConstraints(float boundsWidth, float boundsHeight)
{
    width = boundsWidth;
    height = boundsHeight;
}
