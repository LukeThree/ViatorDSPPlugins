/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhonemodelAudioProcessorEditor::PhonemodelAudioProcessorEditor (PhonemodelAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
,bandwithDial(" %", 0.0, 100.0, 1.0, 70.0, audioProcessor.treeState, bandwidthID)
,centerDial(" Hz", 100.0, 3000.0, 1.0, 800.0, audioProcessor.treeState, centerID)
,driveDial(" dB", 0.0, 12.0, 0.1, 4.0, audioProcessor.treeState, driveID)
,resoDial(" %", 0.0, 100.0, 1.0, 70.0, audioProcessor.treeState, resoID)
,lowShelfDial(" dB", 0.0, 12.0, 0.25, 0.0, audioProcessor.treeState, lowShelfID)
,highShelfDial(" dB", 0.0, 12.0, 0.25, 0.0, audioProcessor.treeState, highShelfID)
{
    uiConstructor();
}

PhonemodelAudioProcessorEditor::~PhonemodelAudioProcessorEditor()
{
}

//==============================================================================
void PhonemodelAudioProcessorEditor::paint (juce::Graphics& g)
{
    
}

void PhonemodelAudioProcessorEditor::resized()
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    uiResized(width, height);
}
