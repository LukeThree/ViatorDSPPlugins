/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiQAudioProcessorEditor::MultiQAudioProcessorEditor (MultiQAudioProcessor& p)
    : AudioProcessorEditor (&p),
audioProcessor (p),
windowComponent(audioProcessor),
graphicEQComponent(audioProcessor),
parametricEQComponent(audioProcessor),
tubeEQComponent(audioProcessor),
hpFaderLabel(true, "HP"), lpFaderLabel(true, "LP"), driveFaderLabel(true, "Drive"), trimFaderLabel(true, "Trim")
, graphicEQButton(true, "Graphic"), paraEQButton(true, "Parametric"), tubeEQButton(true, "Tube")
{
    uiConstructor();
}

MultiQAudioProcessorEditor::~MultiQAudioProcessorEditor()
{
    for (auto& fader : faders)
    {
        fader = nullptr;
    }
    
    faders.clear();
    faders.shrink_to_fit();
    
    for (auto& label : labels)
    {
        label = nullptr;
    }
    
    labels.clear();
    labels.shrink_to_fit();
    
    labelTexts.clear();
    labelTexts.shrink_to_fit();
    
    hpFaderAttach = nullptr;
    lpFaderAttach = nullptr;
    driveFaderAttach = nullptr;
    trimFaderAttach = nullptr;
    
    graphicButtonAttach = nullptr;
    parametricButtonAttach = nullptr;
    tubeButtonAttach = nullptr;
}

//==============================================================================
void MultiQAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void MultiQAudioProcessorEditor::resized()
{
    uiResized();
}
