/*
  ==============================================================================

    SliderGroup.cpp
    Created: 18 Apr 2022 11:36:16am
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderGroup.h"

//==============================================================================
SliderGroup::SliderGroup(MidiMapTestAudioProcessor& p) : audioProcessor(p),
fader1(" dB", "Fader 1", 0.0, 10.0, 0.01, 0.0)
, fader2(" dB", "Fader 2", 0.0, 10.0, 0.01, 0.0)
, fader3(" dB", "Fader 3", 0.0, 10.0, 0.01, 0.0)
, fader4(" dB", "Fader 4", 0.0, 10.0, 0.01, 0.0)
, macroDial("", "Macro", 0.0, 10.0, 0.01, 0.0)
{
    addAndMakeVisible(fader1);
    fader1.setComponentID("p1");
    fader1.addListener(this);
    p1Attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, fader1.getComponentID(), fader1);

    addAndMakeVisible(fader2);
    fader2.setComponentID("p2");
    fader2.addListener(this);
    p2Attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, fader2.getComponentID(), fader2);
    
    addAndMakeVisible(fader3);
    fader3.setComponentID("p3");
    fader3.addListener(this);
    p3Attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, fader3.getComponentID(), fader3);
    
    addAndMakeVisible(fader4);
    fader4.setComponentID("p4");
    fader4.addListener(this);
    p4Attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, fader4.getComponentID(), fader4);
    
    addAndMakeVisible(border);
    
    addAndMakeVisible(macroDial);
    macroDial.setComponentID("macrodial");
    macroAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, macroDial.getComponentID(), macroDial);
    
    addAndMakeVisible(macroMapButton);
    macroMapButton.setClickingTogglesState(false);
    macroMapButton.setButtonText("Map");
    macroMapButton.onClick = [this]()
    {
        if (macroLearnMode)
        {
            audioProcessor.macroMap.setMacro1Group(getLastSliderMovedID());
        }
    };
}

SliderGroup::~SliderGroup()
{
}

void SliderGroup::paint (juce::Graphics& g)
{
}

void SliderGroup::resized()
{
    auto leftMargin = getWidth() * 0.05;
    auto topMargin = getHeight() * 0.1;
    auto faderHeight = getHeight() * 0.8;
    auto faderWidth = getWidth() * 0.15;
    auto dialSize = getWidth() * 0.25;
    
    fader1.setBounds(leftMargin, topMargin, faderWidth, faderHeight);
    fader2.setBounds(fader1.getX() + fader1.getWidth(), topMargin, faderWidth, faderHeight);
    fader3.setBounds(fader2.getX() + fader2.getWidth(), topMargin, faderWidth, faderHeight);
    fader4.setBounds(fader3.getX() + fader3.getWidth(), topMargin, faderWidth, faderHeight);
    macroDial.setBounds(fader4.getX() + fader4.getWidth(), topMargin, dialSize, dialSize);
    macroMapButton.setBounds(macroDial.getX(), macroDial.getY() + macroDial.getHeight(), macroDial.getWidth(), macroDial.getWidth() * 0.33);
    border.setBounds(getLocalBounds());
}

void SliderGroup::sliderValueChanged(juce::Slider *slider)
{
    lastSliderMovedID = slider->getComponentID();
    DBG(slider->getComponentID());
}

juce::String SliderGroup::getLastSliderMovedID()
{
    return lastSliderMovedID;
}
