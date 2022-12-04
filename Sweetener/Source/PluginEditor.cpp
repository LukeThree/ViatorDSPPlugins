/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SweetenerAudioProcessorEditor::SweetenerAudioProcessorEditor (SweetenerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(_multiBandWindow);
    initWindow();
}

SweetenerAudioProcessorEditor::~SweetenerAudioProcessorEditor()
{
}

//==============================================================================
void SweetenerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.75f), getHeight(), juce::Colour::fromRGB(40, 42, 53).brighter(0.02f), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
}

void SweetenerAudioProcessorEditor::resized()
{
    const auto windowLeftMargin = getWidth() * 0.05;
    const auto windowTopMargin = getHeight() * 0.1;
    const auto windowWidth = getWidth() * 0.9;
    const auto windowHeight = getHeight() * 0.5;
    
    _multiBandWindow.setBounds(windowLeftMargin, windowTopMargin, windowWidth, windowHeight);
}

void SweetenerAudioProcessorEditor::initWindow()
{
    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
    
    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();
    
    auto width = 0;
    
    if (r.getWidth() <= 1920)
    {
        width = x * 0.9;
    }
    
    else
    {
        width = x * 0.3;
    }
    
    auto height = width * 0.5;
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width * 0.5, height * 0.5, width * 1.25, height * 1.25);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
    
    if (audioProcessor.windowWidth != 0.0)
    {
        setSize(audioProcessor.windowWidth, audioProcessor.windowHeight);
    }
        
    else
    {
        setSize (width, height);
    }
}
