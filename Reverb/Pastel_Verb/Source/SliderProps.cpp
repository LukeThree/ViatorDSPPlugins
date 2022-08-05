/*
  ==============================================================================

    SliderProps.cpp
    Created: 21 Aug 2021 12:56:03pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void Pastel_VerbAudioProcessorEditor::setSliderProps()
{
    // Cutoff
    cutoffSlider.setTextValueSuffix(" Hz");
    cutoffSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, cutoffSliderId, cutoffSlider);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setSkewFactorFromMidPoint(750);
    cutoffSlider.setDoubleClickReturnValue(true, 20000.0);
    cutoffSlider.setEnabled(filterEngageButton.getToggleState());
    
    // Room size
    roomSizeSlider.setTextValueSuffix(" %");
    roomSizeSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, roomSizeSliderId, roomSizeSlider);
    roomSizeSlider.setRange(0.0, 100.0, 1.0);

    // Resonance
    resonanceSlider.setTextValueSuffix(" %");
    resonanceSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, resonanceSliderId, resonanceSlider);
    resonanceSlider.setRange(0.0, 100.0, 1.0);
    resonanceSlider.setEnabled(filterEngageButton.getToggleState());

    // Damping
    dampingSlider.setTextValueSuffix(" %");
    dampingSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, dampingSliderId, dampingSlider);
    dampingSlider.setRange(0.0, 100.0, 1.0);

    // Drive
    driveSlider.setTextValueSuffix(" dB");
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);
    driveSlider.setRange(0.0, 24.0, 0.5);
    driveSlider.setEnabled(filterEngageButton.getToggleState());

    // Width
    widthSlider.setTextValueSuffix(" %");
    widthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, widthSliderId, widthSlider);
    widthSlider.setRange(0.0, 100.0, 1.0);

    // Dry
    drySlider.setTextValueSuffix(" %");
    drySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, drySliderId, drySlider);
    drySlider.setRange(0.0, 103.0, 1.0);
    drySlider.addListener(this);

    // Wet
    wetSlider.setTextValueSuffix(" %");
    wetSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, wetSliderId, wetSlider);
    wetSlider.setRange(0.0, 103.0, 1.0);
    wetSlider.addListener(this);

}
