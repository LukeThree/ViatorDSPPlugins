/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ElectronEQAudioProcessorEditor::ElectronEQAudioProcessorEditor (ElectronEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setShadowProps();
    setStereoBoxProps(stereoMenu);
    initButtons();
    initDials();
    initLabels();
    
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
    int x = r.getWidth();
    auto width = x / 2.0;
    auto height = width / 2.0;
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width / 2.0, height / 2.0, width * 1.5, height * 1.5);
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

ElectronEQAudioProcessorEditor::~ElectronEQAudioProcessorEditor()
{
    for (auto i {0}; i < sliders.size(); i++)
    {
        sliders[i]->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void ElectronEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    //Image layer from Illustrator
    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::pluginBackground2_png, BinaryData::pluginBackground2_pngSize);
    g.drawImageWithin(pluginBackground, 0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::RectanglePlacement::stretchToFit);
            
    // Logo
    headerLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    g.drawImageWithin(headerLogo, AudioProcessorEditor::getWidth() * 0.4, AudioProcessorEditor::getHeight() * 0.8 + 4, AudioProcessorEditor::getWidth() * 0.25, AudioProcessorEditor::getHeight() * 0.1, juce::RectanglePlacement::centred);
    
    // Header rectangle
    juce::Rectangle<float> footer;
    footer.setBounds(AudioProcessorEditor::getWidth() * 0.4, AudioProcessorEditor::getHeight() * 0.8 + 4, AudioProcessorEditor::getWidth() * 0.25, AudioProcessorEditor::getHeight() * 0.1);
    g.setColour(juce::Colours::black.withAlpha(0.35f));
    g.fillRect(footer);
}

void ElectronEQAudioProcessorEditor::resized()
{
    auto topMargin {AudioProcessorEditor::getHeight() * 0.15};
    auto height {AudioProcessorEditor::getHeight()};
    auto leftMargin {AudioProcessorEditor::getWidth() / 12.0};
    auto buttonWidth {AudioProcessorEditor::getWidth() / 16.0};
    auto buttonHeight {AudioProcessorEditor::getHeight() / 12.0};
    auto dialSize {height / 3.5};
    auto spaceBetweenDials {1.1};
    auto smallDialSize {dialSize * 0.85};
    
    lowBoostDial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    lowCutDial.setBounds(lowBoostDial.getX() + lowBoostDial.getWidth() * spaceBetweenDials, topMargin, dialSize, dialSize);
    highBoostDial.setBounds(lowCutDial.getX() + lowCutDial.getWidth() * spaceBetweenDials, topMargin, dialSize, dialSize);
    highCutDial.setBounds(highBoostDial.getX() + highBoostDial.getWidth() * spaceBetweenDials, topMargin, dialSize, dialSize);
    
    lowFreqDial.setBounds(lowBoostDial.getX() + (lowBoostDial.getWidth() / 1.55), lowBoostDial.getY() + lowBoostDial.getHeight() * 1.2, smallDialSize, smallDialSize);
    bandwidthDial.setBounds(lowFreqDial.getX() + lowFreqDial.getWidth() * 1.3, lowFreqDial.getY(), smallDialSize, smallDialSize);
    highFreqDial.setBounds(highBoostDial.getX() + (highBoostDial.getWidth() / 1.55), bandwidthDial.getY(), smallDialSize, smallDialSize);
    
    enabledToggle.setBounds(lowBoostDial.getX() + (lowBoostDial.getWidth() / 4.0), lowFreqDial.getY() + lowFreqDial.getHeight() * 1.35, buttonWidth, buttonHeight);
    enabledToggle.changeWidthToFitText();
    
    oversampleToggle.setBounds(enabledToggle.getX() + enabledToggle.getWidth(), enabledToggle.getY(), buttonWidth, buttonHeight);
    
    lowPassSlider.setBounds(highCutDial.getX() + highCutDial.getWidth(), highCutDial.getY(), height / 8.0, height * 0.6);
    driveSlider.setBounds(lowPassSlider.getX() + lowPassSlider.getWidth() * 1.25, lowPassSlider.getY(), lowPassSlider.getWidth(), lowPassSlider.getHeight());
    trimSlider.setBounds(driveSlider.getX() + driveSlider.getWidth() * 1.25, driveSlider.getY(), driveSlider.getWidth(), driveSlider.getHeight());
    
    // Sample menu
    stereoMenu.setBounds(driveSlider.getX(), lowPassSlider.getY() + lowPassSlider.getHeight() * 1.12, AudioProcessorEditor::getWidth() * 0.15, AudioProcessorEditor::getHeight() * 0.06);
    
    // Save plugin size in value tree
    audioProcessor.variableTree.setProperty("width", getWidth(), nullptr);
    audioProcessor.variableTree.setProperty("height", getHeight(), nullptr);
}

void ElectronEQAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &lowFreqDial)
    {
        /**Lock the low freq dial at 22 so the user doesn't get bothered by the skew jump*/
        if (slider->getValue() <= 22.0)
        {
            slider->setValue(22.0);
        }
    }
}
