/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiMapTestAudioProcessorEditor::MidiMapTestAudioProcessorEditor (MidiMapTestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), sliderGroup(audioProcessor)

{
    addAndMakeVisible(sliderGroup);
    addAndMakeVisible(newComp);
    
    addAndMakeVisible(learnButton);
    learnButton.setButtonText("Learn");
    learnButton.onClick = [this]()
    {
        sliderGroup.setMidiLearnMode(learnButton.getToggleState());
    };
    //learnAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "learn", learnButton);
    
    addAndMakeVisible(macroLearnButton);
    macroLearnButton.setButtonText("Macro Learn");
    macroLearnButton.onClick = [this]()
    {
        sliderGroup.setMacroLearnMode(macroLearnButton.getToggleState());
    };
    
    setSize (1200, 600);
}

MidiMapTestAudioProcessorEditor::~MidiMapTestAudioProcessorEditor()
{
}

//==============================================================================
void MidiMapTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("MIDI", getLocalBounds(), juce::Justification::centredTop, 1);
}

void MidiMapTestAudioProcessorEditor::resized()
{
    auto topMargin = getHeight() * 0.1;
    auto faderWidth = getWidth() * 0.1;
    
    sliderGroup.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth() * 0.75, getHeight() * 0.8).withX(12));
    
    learnButton.setBounds(sliderGroup.getX() + sliderGroup.getWidth() + 12, topMargin, faderWidth, faderWidth);
    macroLearnButton.setBounds(learnButton.getX(), learnButton.getY() + learnButton.getHeight(), learnButton.getWidth(), learnButton.getHeight());
    
    newComp.setBounds(learnButton.getX() + learnButton.getWidth(), learnButton.getY(), learnButton.getWidth(), learnButton.getHeight());
}
