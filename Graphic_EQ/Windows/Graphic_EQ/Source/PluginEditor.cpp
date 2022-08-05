/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicEQAudioProcessorEditor::GraphicEQAudioProcessorEditor (GraphicEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    uiConstructor();
}

GraphicEQAudioProcessorEditor::~GraphicEQAudioProcessorEditor()
{
    // Clear vector memory allocation
    sliders.reserve(0);
    labels.reserve(0);
    buttons.reserve(0);
    
    // This fixes a memory leak when you close the window
    for (auto i = 0; i < sliders.size(); ++i)
    {
        sliders[i]->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void GraphicEQAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void GraphicEQAudioProcessorEditor::resized()
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    uiResized(width, height);
}

void GraphicEQAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    
}

void GraphicEQAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &shadowToggle)
    {
        // Run through all sliders
        for (int i = 0; i < sliders.size(); ++i)
        {
            if (shadowToggle.getToggleState())
            {
                sliders[i]->setComponentEffect(nullptr);
            }
            
            else
            {
                sliders[i]->setComponentEffect(&dialShadow);
            }
        }
    }
}
