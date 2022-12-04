/*
  ==============================================================================

    MultiBandWindow.cpp
    Created: 4 Dec 2022 2:38:09pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MultiBandWindow.h"

//==============================================================================
MultiBandWindow::MultiBandWindow()
{
    _bandSlider.setRange(20.0, 20000.0, 1.0);
    _bandSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    _bandSlider.setLookAndFeel(&_windowSliderStyle);
    addAndMakeVisible(_bandSlider);
    
    _bandSlider.onValueChange = [this]()
    {
        _bandSlider.setPopupPosition();
    };
}

MultiBandWindow::~MultiBandWindow()
{
    _bandSlider.setLookAndFeel(nullptr);
}

void MultiBandWindow::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.brighter(0.1));
}

void MultiBandWindow::resized()
{
    _bandSlider.setBounds(0, 0, getWidth(), getHeight());
}
