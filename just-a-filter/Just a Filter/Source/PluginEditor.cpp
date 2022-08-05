/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JustaFilterAudioProcessorEditor::JustaFilterAudioProcessorEditor (JustaFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
headerComponent(audioProcessor.treeState),
filterComponent(audioProcessor)
{
    uiConstructor();
}

JustaFilterAudioProcessorEditor::~JustaFilterAudioProcessorEditor()
{
}

//==============================================================================
void JustaFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void JustaFilterAudioProcessorEditor::resized()
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    uiResized(width, height);
}
