/*
  ==============================================================================

    LV_HeaderComponent.cpp
    Created: 17 Jan 2022 5:31:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LV_HeaderComponent.h"

//==============================================================================
LV_HeaderComponent::LV_HeaderComponent(juce::AudioProcessorValueTreeState& tree)
: trimSlider(" Trim", -24.0, 24.0, 0.1, 0.0)
{
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    oversamplingMenu.addItem("No OS", 1);
    oversamplingMenu.addItem("8X", 2);
    oversamplingMenuAttach = std::make_unique<ComboBoxAttachment>(tree, qualityID, oversamplingMenu);
    addAndMakeVisible(oversamplingMenu);
    
    bandwidthTypeMenu.addItem("Linear Q", 1);
    bandwidthTypeMenu.addItem("Analog Q", 2);
    bandwidthTypeMenuAttach = std::make_unique<ComboBoxAttachment>(tree, bandwidthTypeID, bandwidthTypeMenu);
    addAndMakeVisible(bandwidthTypeMenu);
    
    filterTypeMenu.addItem("Low Shelf", 1);
    filterTypeMenu.addItem("Low Cut", 2);
    filterTypeMenu.addItem("Notch", 3);
    filterTypeMenu.addItem("High Cut", 4);
    filterTypeMenu.addItem("High Shelf", 5);
    filterTypeMenuAttach = std::make_unique<ComboBoxAttachment>(tree, filterTypeID, filterTypeMenu);
    addAndMakeVisible(filterTypeMenu);
    
    trimAttach = std::make_unique<SliderAttachment>(tree, trimID, trimSlider);
    addAndMakeVisible(trimSlider);
    
    trimReset.onClick = [this]() 
    {
        trimSlider.setValue(0.0);
    };
    
    trimReset.setButtonText("Reset");
    trimReset.setClickingTogglesState(false);
    addAndMakeVisible(trimReset);
}

LV_HeaderComponent::~LV_HeaderComponent()
{
    oversamplingMenuAttach = nullptr;
    filterTypeMenuAttach = nullptr;
    bandwidthTypeMenuAttach = nullptr;
    trimAttach = nullptr;
}

void LV_HeaderComponent::paint (juce::Graphics& g)
{
    // Header rectangle
    juce::Rectangle<float> header;
    header.setBounds(0, 0, width, height);
    g.setColour(juce::Colours::black.brighter(0.1).withAlpha(0.8f));
    g.fillRect(header);

    // Logo layer
    footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);

    // Draw and position the image
    g.drawImageWithin(footerLogo, width * 0.025, 4.0f, getHeight() * 3.5f, getHeight() * 0.75, juce::RectanglePlacement::stretchToFit);

    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(width * 0.38, height * 0.9, width * 0.25, height * 0.1);
}

void LV_HeaderComponent::resized()
{
    float rightMargin = width * 0.99f;
    float spaceBetween = width * 0.01f;
    float topMargin = getHeight() * 0.125f;
    float menuWidth = width * 0.15f;
    float menuHeight = getHeight() * 0.75f;
    
    oversamplingMenu.setBounds(rightMargin - menuWidth, topMargin, menuWidth, menuHeight);
    bandwidthTypeMenu.setBounds(oversamplingMenu.getX() - spaceBetween - oversamplingMenu.getWidth(), topMargin, menuWidth, menuHeight);
    filterTypeMenu.setBounds(bandwidthTypeMenu.getX() - spaceBetween - bandwidthTypeMenu.getWidth(), topMargin, menuWidth, menuHeight);
    trimSlider.setBounds(filterTypeMenu.getX() - spaceBetween - bandwidthTypeMenu.getWidth() / 1.5, topMargin, menuWidth / 1.5, menuHeight);
    trimReset.setBounds(trimSlider.getX() - spaceBetween - trimSlider.getWidth(), topMargin, trimSlider.getWidth(), menuHeight);
}

void LV_HeaderComponent::setWidthAndHeight(float w, float h)
{
    width = w;
    height = h;
}
