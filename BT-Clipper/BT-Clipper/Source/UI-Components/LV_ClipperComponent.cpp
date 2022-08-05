/*
  ==============================================================================

    LV_ClipperComponent.cpp
    Created: 14 Jan 2022 5:33:49pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_ClipperComponent.h"

//==============================================================================
LV_ClipperComponent::LV_ClipperComponent(BTClipperAudioProcessor& p) : audioProcessor(p)
{
    // Drive
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    powerAttach = std::make_unique<ButtonAttachment>(audioProcessor.treeState, clipperPowerID, moduleBypass);
    addAndMakeVisible(moduleBypass);
    moduleBypass.setToggleStyle(juce::LV_Toggle::ToggleStyle::kPower);
    
    driveFaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, driveID, driveFader);
    addAndMakeVisible(driveFader);
    driveFader.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.brighter(0.25f));
    driveFader.forceShadow();
    addAndMakeVisible(driveFaderLabel);
    driveFaderLabel.attachToComponent(&driveFader, false);
    driveFaderLabel.setText("Drive", juce::dontSendNotification);
    
    addAndMakeVisible(clipGroup);
    clipGroup.setText("Clipper");
    
    // Ceiling Dial
    ceilingDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, ceilingID, ceilingDial);
    addAndMakeVisible(ceilingDial);
    ceilingDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.brighter(0.15f));
    ceilingDial.forceShadow();
    addAndMakeVisible(ceilingDialLabel);
    ceilingDialLabel.attachToComponent(&ceilingDial, false);
    ceilingDialLabel.setText("Ceiling", juce::dontSendNotification);
    
    // Mix Dial
    mixDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, mixID, mixDial);
    addAndMakeVisible(mixDial);
    mixDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.brighter(0.15f));
    mixDial.forceShadow();
    addAndMakeVisible(mixDialLabel);
    mixDialLabel.attachToComponent(&mixDial, false);
    mixDialLabel.setText("Mix", juce::dontSendNotification);
    
    // Trim Dial
    trimDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, trimID, trimDial);
    addAndMakeVisible(trimDial);
    trimDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.brighter(0.15f));
    trimDial.forceShadow();
    addAndMakeVisible(trimDialLabel);
    trimDialLabel.attachToComponent(&trimDial, false);
    trimDialLabel.setText("Trim", juce::dontSendNotification);
    
    // Scope stuff
    addAndMakeVisible(scopeGroup);
    
    addAndMakeVisible(audioProcessor.scopeModule);
    audioProcessor.scopeModule.setOpaque(false);
    audioProcessor.scopeModule.setColours(scopeColor.withAlpha(0.0f), juce::Colours::black.brighter(0.1).withAlpha(0.75f));
}

LV_ClipperComponent::~LV_ClipperComponent()
{
    driveFaderAttach = nullptr;
    ceilingDialAttach = nullptr;
    trimDialAttach = nullptr;
    mixDialAttach = nullptr;
    powerAttach = nullptr;
}

void LV_ClipperComponent::paint (juce::Graphics& g)
{
    auto leftMargin = getWidth() * 0.05;
    auto topMargin = getWidth() * 0.1;
    auto wider = 0.25;
    
    osc = juce::ImageCache::getFromMemory(BinaryData::oscilloscope01_png, BinaryData::oscilloscope01_pngSize);
    g.drawImageWithin(osc, leftMargin * 8.0, topMargin * 0.8, width * wider, width * wider, juce::RectanglePlacement::stretchToFit);
}

void LV_ClipperComponent::resized()
{
    auto leftMargin = getWidth() * 0.05;
    auto topMargin = getWidth() * 0.1;
    auto dialSize = width * 0.1;
    auto spaceBetween = 1.15;
    auto dialSpaceBetween = 1.65;
    auto wider = 0.225;
    auto higer = 0.206;
    
    // Clip stuff
    clipGroup.setBounds(0, 0, getWidth(), getHeight());
    moduleBypass.setBounds(clipGroup.getX() + leftMargin / 3, clipGroup.getY() + topMargin / 3, 48, 48);
    driveFader.setBounds(clipGroup.getX() + leftMargin, clipGroup.getY() + topMargin * 2.0, dialSize, clipGroup.getHeight() * 0.45);
    ceilingDial.setBounds(driveFader.getX(), driveFader.getY() + driveFader.getHeight() * spaceBetween, dialSize, dialSize);
    mixDial.setBounds(ceilingDial.getX() + ceilingDial.getWidth() * dialSpaceBetween, ceilingDial.getY(), dialSize, dialSize);
    trimDial.setBounds(mixDial.getX() + mixDial.getWidth() * dialSpaceBetween, mixDial.getY(), dialSize, dialSize);
    audioProcessor.scopeModule.setBounds(leftMargin * 8.5, topMargin * 1.35, width * wider, width * higer);
}

void LV_ClipperComponent::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
