/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoxDriveAudioProcessorEditor::VoxDriveAudioProcessorEditor (VoxDriveAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
, headerComponent(audioProcessor)
, settingsPage(audioProcessor)
, driveDial(" dB", "Drive", 0.0, 50.0, 0.01, 0.0)
, rangeDial(" Hz", "Range", 500.0, 20000.0, 0.0, 500.0)
, lowpassDial(" Hz", "LP", 1000.0, 20000.0, 1.0, 1000.0)
, trimDial(" dB", "Trim", -24.0, 24.0, 0.01, 0.0)
, outDial(" dB", "Output", -24.0, 24.0, 0.01, 0.0)
, mixFader(" %", "Mix", 0.0, 100.0, 1.0, 0.0)
, osButton(true, "HQ")
, phaseButton(true, "Phase")
{
    uiConstructor();
}

VoxDriveAudioProcessorEditor::~VoxDriveAudioProcessorEditor()
{
    //sliders
    for (int i = 0; i < sliders.size(); ++i)
    {
       sliders[i]->setLookAndFeel(nullptr);
    }
    
    //buttons
    for (int i = 0; i < buttons.size(); ++i)
    {
        buttons[i]->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void VoxDriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void VoxDriveAudioProcessorEditor::resized()
{
    uiResized();
}

void VoxDriveAudioProcessorEditor::mouseEnter(const juce::MouseEvent &event)
{
    //sliders
    for (int i = 0; i < sliders.size(); ++i)
    {
        if (event.eventComponent == sliders[i])
        {
            tooltipContent.setText(sliderLabels[i], juce::dontSendNotification);
        }
    }
    
    //buttons
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (event.eventComponent == buttons[i])
        {
            tooltipContent.setText(buttonLabels[i], juce::dontSendNotification);
        }
    }
}

void VoxDriveAudioProcessorEditor::mouseExit(const juce::MouseEvent &event)
{
    //sliders
    for (int i = 0; i < sliders.size(); ++i)
    {
        if (event.eventComponent == sliders[i])
        {
            tooltipContent.setText("", juce::dontSendNotification);
        }
    }
    
    //buttons
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (event.eventComponent == buttons[i])
        {
            tooltipContent.setText("", juce::dontSendNotification);
        }
    }
}
