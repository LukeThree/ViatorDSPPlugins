/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void GraphicEQAudioProcessorEditor::uiConstructor()
{
    // Window
    initWindow();
    addAndMakeVisible(windowComponent);
    
    // Initialize shadows
    setShadowProps();
    
    // Initialize component properties
    setAllProps();
    
    // Slider one
    band1SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, oneId, band1Slider);
    band1Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider two
    band2SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, twoId, band2Slider);
    band2Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider three
    band3SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, threeId, band3Slider);
    band3Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider four
    band4SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, fourId, band4Slider);
    band4Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider five
    band5SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, fiveId, band5Slider);
    band5Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider six
    band6SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, sixId, band6Slider);
    band6Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider seven
    band7SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, sevenId, band7Slider);
    band7Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider eight
    band8SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, eightId, band8Slider);
    band8Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider nine
    band9SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, nineId, band9Slider);
    band9Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Slider ten
    band10SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, tenId, band10Slider);
    band10Slider.setRange(-12.0f, 12.0f, 0.25);
    
    // Drive slider
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    driveSlider.setLookAndFeel(&customDial);
    driveSlider.setComponentEffect(&dialShadow);
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveId, driveSlider);
    driveSlider.setRange(0.0, 10.0);
    
    // Trim Slider
    trimSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    trimSlider.setLookAndFeel(&customDial);
    trimSlider.setComponentEffect(&dialShadow);
    trimSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
    trimSlider.setRange(-24.0, 24.0, 1.0);
    
    // Phase button
    phaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseId, phaseToggle);
    
    // Low bell button
    lowBellAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, lowBellId, lowBellButton);
    
    // High bell button
    highBellAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, highBellId, highBellButton);
}
