/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExciterAudioProcessorEditor::ExciterAudioProcessorEditor (ExciterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, amountDial(" %", "Amount", 0.0, 100.0, 1.0, 0.0)
, rangeDial(" Hz", "Range", 1000.0, 20000.0, 1.0, 15000.0)
, oddDial(" dB", "Odd", 0.0, 10.0, 0.01, 0.0)
, evenDial(" dB", "Even", 0.0, 10.0, 0.01, 0.0)
, mixFader(" %", "Mix", 0.0, 1.0, 0.01, 0.0)
, trimFader(" dB", "Trim", -24.0, 24.0, 0.01, 0.0)
, phaseToggle("Phase")
, osToggle("Oversample")
{
    uiConstructor();
}

ExciterAudioProcessorEditor::~ExciterAudioProcessorEditor()
{
}

//==============================================================================
void ExciterAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void ExciterAudioProcessorEditor::resized()
{
    uiResized(getWidth(), getHeight());
}

void ExciterAudioProcessorEditor::drawFrame(juce::Graphics &g)
{
    for (int i = 1; i < audioProcessor.scopeSize; ++i)
    {
        auto width  = spectrumWindow.toType<int>().getWidth();
        auto height = spectrumWindow.toType<int>().getHeight() * 0.9;
        
        int x = spectrumWindow.getX();
        int y = spectrumWindow.getY() + spectrumWindow.getHeight() * 0.05;
        
        g.setColour(juce::Colours::whitesmoke.withAlpha(0.5f));
        g.drawLine ({ (float) juce::jmap (i - 1, 0, audioProcessor.scopeSize - 1, x, width + x),
                                juce::jmap (audioProcessor.scopeData[i - 1], 0.0f, 1.0f, (float) height + (float) y, (float) y),
                        (float) juce::jmap (i,     0, audioProcessor.scopeSize - 1, x, width + x),
                                juce::jmap (audioProcessor.scopeData[i],     0.0f, 1.0f, (float) height + (float) y, (float) y), });
    }
}
