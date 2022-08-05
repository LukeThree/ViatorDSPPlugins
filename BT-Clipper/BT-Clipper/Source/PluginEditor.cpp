/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BTClipperAudioProcessorEditor::BTClipperAudioProcessorEditor (BTClipperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
headerComponent(audioProcessor.treeState),
preToneComponent(audioProcessor),
clipperComponent(audioProcessor),
postComponent(audioProcessor)
{
    uiConstructor();
}

BTClipperAudioProcessorEditor::~BTClipperAudioProcessorEditor()
{
    
}

//==============================================================================
void BTClipperAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void BTClipperAudioProcessorEditor::resized()
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    uiResized(width, height);
}
