/*
  ==============================================================================

    InitSliders.cpp
    Created: 9 Oct 2021 2:11:32am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::init_dials()
{
    inputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputSliderId, m_InputSlider);
    outputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputSliderId, m_OutputSlider);
    cutoffSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, cutoffSliderId, m_CutoffDial);
    midGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, midGainSliderId, m_MidToneGain);
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, m_DriveDial);
    
    addAndMakeVisible(m_InputSlider);
    m_InputSlider.setLookAndFeel(&customSlider);
    m_InputSlider.setComponentEffect(&sliderShadow);
    
    addAndMakeVisible(m_OutputSlider);
    m_OutputSlider.setLookAndFeel(&customSlider);
    m_OutputSlider.setComponentEffect(&sliderShadow);
    
    for (auto i {0}; i < sliders.size(); i++)
    {
        addAndMakeVisible(sliders[i]);
        sliders[i]->addListener(this);
        sliders[i]->setLookAndFeel(&customDial);
        sliders[i]->setComponentEffect(&dialShadow);
    }
}
