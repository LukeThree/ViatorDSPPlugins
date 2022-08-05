#include <JuceHeader.h>
#include "LV_Window.h"

//=============================================================================
LV_Window::LV_Window(MultiQAudioProcessor& p) : audioProcessor(p), cpuLabel(false, "CPU"), cpuHeader(true, "CPU")
{
    startTimerHz(10);
    
    addAndMakeVisible(phaseToggle);
    phaseButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseID, phaseToggle);
   
    addAndMakeVisible(osMenu);
    osMenu.setText("Quality");
    osMenu.addItem("Normal", 1);
    osMenu.addItem("High", 2);
    osAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, qualityID, osMenu);
   
    addAndMakeVisible(stereoMenu);
    stereoMenu.setText("M / S");
    stereoMenu.addItem("Stereo", 1);
    stereoMenu.addItem("Mid", 2);
    stereoMenu.addItem("Sides", 3);
    msAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, msID, stereoMenu);
    
    addAndMakeVisible(cpuLabel);
    addAndMakeVisible(cpuHeader);
    addAndMakeVisible(cpuButton);
    cpuButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "cpu", cpuButton);

}

LV_Window::~LV_Window()
{
    stopTimer();
    phaseButtonAttach = nullptr;
    osAttach = nullptr;
    msAttach = nullptr;
    cpuButtonAttach = nullptr;
}

void LV_Window::paint (juce::Graphics& g)
{
    // Backgroun
    auto background = juce::ImageCache::getFromMemory(BinaryData::pluginBackground2_png, BinaryData::pluginBackground2_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
   
    // Logo laye
    auto footerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
   
    // Draw and position the imag
    g.drawImageWithin(footerLogo, getWidth() * 0.075, getHeight() * 0.83, getWidth() * 0.125, getHeight() * 0.05, juce::RectanglePlacement::centred);
    
    // Patreon link
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    mWebLink.setBounds(getWidth() * 0.128, getHeight() * 0.8 + 4, getWidth() * 0.25, getHeight() * 0.);
    
//    // Plugin text
//    g.setColour (juce::Colours::whitesmoke.withAlpha(0.125f));
//    g.setFont(getWidth() * 0.015);
//    g.drawFittedText ("Multi-Q v1.1.1", getWidth() * 0.12, getHeight() * 0.84, getWidth(), getHeight()1, juce:Justification::topLeft, 1);
}

void LV_Window::resized()
{
    auto menuWidth = getWidth() * 0.1;
    auto menuHeight = menuWidth * 0.33;
    auto faderWidth = getWidth() * 0.065;
    auto buttonWidth = faderWidth * 1.3333;
    auto buttonheight = buttonWidth * 0.33;
    auto labelSize = getWidth() * 0.04;
    auto labelFont = getWidth() * 0.1 * 0.12f;
    
    osMenu.setBounds(getWidth() * 0.35, getHeight() * 0.825, menuWidth, menuHeight);
    stereoMenu.setBounds(osMenu.getX() + osMenu.getWidth(), osMenu.getY(), menuWidth, menuHeight);
    
    phaseToggle.setBounds(getWidth() * 0.64, getHeight() * 0.84, buttonWidth, buttonheight);
    
    cpuButton.setBounds(getWidth() * 0.8, getHeight() * 0.82, labelSize, labelSize);
    
    cpuLabel.setBounds(cpuButton.getX() + cpuButton.getWidth(), cpuButton.getY(), labelSize, labelSize);
    cpuLabel.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    cpuHeader.setBounds(cpuLabel.getX() + cpuLabel.getWidth(), cpuLabel.getY(), labelSize, labelSize);
    cpuHeader.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
}


