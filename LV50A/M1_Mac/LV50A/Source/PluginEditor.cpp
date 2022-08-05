/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LV50AAudioProcessorEditor::LV50AAudioProcessorEditor (LV50AAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    uiConstructor();
}

LV50AAudioProcessorEditor::~LV50AAudioProcessorEditor()
{
    verticalSliders.reserve(0);
    filterFreqSliders.reserve(0);
    filterSliders.reserve(0);

    outLabels.reserve(0);
    filterLabels.resize(0);
    buttons.reserve(0);
    
    // This fixes a memory leak when you close the window
    for (auto i = 0; i < verticalSliders.size(); ++i)
    {
        verticalSliders[i]->setLookAndFeel(nullptr);
    }
    
    for (auto i = 0; i < filterSliders.size(); ++i)
    {
        filterSliders[i]->setLookAndFeel(nullptr);
    }
    
    for (auto i = 0; i < filterFreqSliders.size(); ++i)
    {
        filterFreqSliders[i]->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void LV50AAudioProcessorEditor::paint (juce::Graphics& g)
{
//    //Image layer from Illustrator
//    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::pluginBackground_png, BinaryData::pluginBackground_pngSize);
//    g.drawImageWithin(pluginBackground, 0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::RectanglePlacement::stretchToFit);
//
//    // Background dark-maker
//    juce::Rectangle<float> backgroundDarker;
//    backgroundDarker.setBounds(0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight());
//    g.setColour(juce::Colour::fromFloatRGBA(0.09f, 0.10f, 0.12f, 0.65f));
//    g.fillRect(backgroundDarker);
//
//    // Header rectangle
//    juce::Rectangle<float> header;
//    header.setBounds(0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight() * 0.13f);
//    g.setColour(juce::Colours::black.withAlpha(0.5f));
//    g.fillRect(header);
//
//    // Set header text
//    g.setFont (16.0f);
//    g.setColour (juce::Colours::white.withAlpha(0.25f));
//    g.drawFittedText ("Algorithms by Landon Viator", 12, AudioProcessorEditor::getHeight() * 0.05, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::Justification::topLeft, 1);
//
//    // Logo
//    mLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
//    g.drawImageWithin(mLogo, AudioProcessorEditor::getWidth() * 0.4f, AudioProcessorEditor::getHeight() * 0.025, AudioProcessorEditor::getHeight() * 0.08f * 4.58, AudioProcessorEditor::getHeight() * 0.08f, juce::RectanglePlacement::centred);
//
//    // Sample menu
//    mSampleMenu.setBounds(AudioProcessorEditor::getWidth() * 0.8f, AudioProcessorEditor::getHeight() * 0.03, driveSlider.getWidth(), lowShelfToggle.getHeight() * 0.85);
//
//
//    // Web link
//    mWebLink.setBounds(AudioProcessorEditor::getWidth() * 0.4f, AudioProcessorEditor::getHeight() * 0.025, AudioProcessorEditor::getHeight() * 0.08f * 4.58, AudioProcessorEditor::getHeight() * 0.08f);
}

void LV50AAudioProcessorEditor::resized()
{
    
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    uiResized(width, height);
}

void LV50AAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    
}

void LV50AAudioProcessorEditor::buttonClicked(juce::Button *button)
{
     if (button == &gainResetButton)
     {
        for (auto i {0}; i < filterSliders.size(); ++i)
        {
            filterSliders[i]->setValue(0.0);
        }
    }
}
