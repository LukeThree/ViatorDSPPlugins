/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultibandstereowidenerAudioProcessorEditor::MultibandstereowidenerAudioProcessorEditor (MultibandstereowidenerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
lowWidthDial(" dB", 0.0, 2.0, 0.01, 1.0),
highWidthDial(" dB", 0.0, 2.0, 0.01, 1.0),
crossoverFader(" Hz", 60.0, 16000.0, 1.0, 1000.0)
{
    uiConstructor();
}

MultibandstereowidenerAudioProcessorEditor::~MultibandstereowidenerAudioProcessorEditor()
{
}

//==============================================================================
void MultibandstereowidenerAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void MultibandstereowidenerAudioProcessorEditor::resized()
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    uiResized(width, height);
}
