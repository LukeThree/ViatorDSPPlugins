/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DarkstarAudioProcessorEditor::DarkstarAudioProcessorEditor (DarkstarAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, dial1(" dB", 0.0, 10.0, 0.1, 0.0)
{
    addAndMakeVisible(dial1);
    dial1.setDialStyle(viator_gui::Dial::DialStyle::kFullDial);
    dial1.forceShadow();
    
    addAndMakeVisible(pedalView);
    pedalView.setButtonText("OD");
    pedalView.setRadioGroupId(1);
    //pedalView.setClickingTogglesState(false);
    
    addAndMakeVisible(ampView);
    ampView.setButtonText("Amp");
    ampView.setRadioGroupId(1);
    //ampView.setClickingTogglesState(false);
    
    addAndMakeVisible(fxView);
    fxView.setButtonText("Fx");
    fxView.setRadioGroupId(1);
    //pedalView.setClickingTogglesState(false);
    
    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
    
    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();
    
    // Plugin window will always initialize to half the screen width by half of that for a rectangle
    auto width = x / 2.0;
    auto height = width / 2.0;
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width * 0.75, height * 0.75, width * 1.25, height * 1.25);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
    
//    if (audioProcessor.windowWidth != 0.0)
//    {
//        setSize(audioProcessor.windowWidth, audioProcessor.windowHeight);
//    }
//
//    else
//    {
        setSize (width, height);
//    }
}

DarkstarAudioProcessorEditor::~DarkstarAudioProcessorEditor()
{
}

//==============================================================================
void DarkstarAudioProcessorEditor::paint (juce::Graphics& g)
{
    background = juce::ImageCache::getFromMemory(BinaryData::dark_blue_png, BinaryData::dark_blue_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void DarkstarAudioProcessorEditor::resized()
{
    auto tabButtonMargin = getWidth() * 0.25;
    auto buttonWidth = getWidth() * 0.1;
    auto buttonHeight = buttonWidth * 0.6;
    
    pedalView.setBounds(tabButtonMargin, 10, buttonWidth, buttonHeight);
    ampView.setBounds(pedalView.getX() + pedalView.getWidth() * 2.0, 10, buttonWidth, buttonHeight);
    fxView.setBounds(ampView.getX() + ampView.getWidth() * 2.0, 10, buttonWidth, buttonHeight);
    dial1.setBounds(ampView.getX(), ampView.getY() + 128, ampView.getWidth() * 2.0, ampView.getWidth() * 2.0);
}
