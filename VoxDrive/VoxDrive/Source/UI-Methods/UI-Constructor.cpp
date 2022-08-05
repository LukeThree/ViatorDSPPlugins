/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void VoxDriveAudioProcessorEditor::uiConstructor()
{
    // Window
    initWindow();
    
    for (int i = 0; i < labels.size(); ++i)
    {
        addAndMakeVisible(labels[i]);
        labels[i]->attachToComponent(sliders[i], false);
    }
    
    //Dials
    addAndMakeVisible(driveDial);
    driveDial.addMouseListener(this, false);
    driveDial.forceShadow();
    driveAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputID, driveDial);
    
    addAndMakeVisible(rangeDial);
    rangeDial.addMouseListener(this, false);
    rangeDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    rangeDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    rangeDial.forceShadow();
    rangeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, cutoffID, rangeDial);
    
    addAndMakeVisible(lowpassDial);
    lowpassDial.addMouseListener(this, false);
    lowpassDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange.darker(0.5));
    lowpassDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::orange.darker(0.5).withAlpha(0.5f));
    lowpassDial.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    lowpassDial.forceShadow();
    lowpassAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowpassID, lowpassDial);
    
    addAndMakeVisible(trimDial);
    trimDial.addMouseListener(this, false);
    trimAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimID, trimDial);
    
    addAndMakeVisible(outDial);
    outDial.addMouseListener(this, false);
    outAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputID, outDial);
    
    addAndMakeVisible(mixFader);
    mixFader.addMouseListener(this, false);
    mixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, mixID, mixFader);
    
    addAndMakeVisible(osButton);
    osButton.addMouseListener(this, false);
    hqAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, hqID, osButton);
    
    addAndMakeVisible(phaseButton);
    phaseButton.addMouseListener(this, false);
    phaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseID, phaseButton);
    
    addAndMakeVisible(headerComponent);
    addAndMakeVisible(settingsPage);
    
    addAndMakeVisible(tooltipContent);
    tooltipContent.setColour(0x1000280, juce::Colours::transparentBlack);
    tooltipContent.setColour(0x1000282, juce::Colours::transparentBlack);
    tooltipContent.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    tooltipContent.setJustificationType(juce::Justification::horizontallyCentred);
    tooltipContent.setFont(juce::Font ("Helvetica", getHeight() * 0.02f, juce::Font::FontStyleFlags::bold));
}
