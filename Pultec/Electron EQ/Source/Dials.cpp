/*
  ==============================================================================

    Dials.cpp
    Created: 13 Jul 2021 12:33:41am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void ElectronEQAudioProcessorEditor::initDials()
{
    for (auto i {0}; i < dials.size(); i++)
    {
        setCommonDialProps(*dials[i]);
    }
    
    for (auto i {0}; i < sliders.size(); i++)
    {
        setCommonSliderProps(*sliders[i]);
    }
}

void ElectronEQAudioProcessorEditor::setCommonDialProps(juce::Slider &dial)
{
    /**Common dial settings*/
    addAndMakeVisible(dial);
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    dial.setColour(0x1001400, juce::Colour::fromFloatRGBA(1.0, 1.0, 1.0, 0.25));
    dial.setColour(0x1001700, juce::Colour::fromFloatRGBA(1.0, 1.0, 1.0, 0.0));
    dial.setColour(0x1001500, juce::Colour::fromFloatRGBA(0.0, 0.0, 0.0, 0.0));
    dial.setRange(0.0, 10.0, 0.25);
    dial.setDoubleClickReturnValue(true, 0.0);
    dial.setLookAndFeel(&customDial);
    
    /**Specific dial settings*/
    
    lowBoostDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowBoostDialId, lowBoostDial);
    lowCutDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowAttenDialId, lowCutDial);

    bandwidthDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, bandwidthDialId, bandwidthDial);

    //lowFreqDial.addListener(this);
    lowFreqDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowFreqDialId, lowFreqDial);
    lowFreqDial.setRange(20, 300, 1);
    lowFreqDial.setSkewFactorFromMidPoint(100);
    lowFreqDial.setTextValueSuffix(" Hz");
    
    highFreqDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highFreqDialId, highFreqDial);
    highFreqDial.setRange(1000, 16000, 1.0);
    highFreqDial.setSkewFactorFromMidPoint(6000);
    highFreqDial.setTextValueSuffix(" Hz");
    highFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 88, 32);
    
    highBoostDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highBoostDialId, highBoostDial);
    highCutDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highAttenDialId, highCutDial);

    /**Apparently this needs to be last or it won't show up*/
    dial.setComponentEffect(&dialShadow);
}

void ElectronEQAudioProcessorEditor::setCommonSliderProps(juce::Slider &slider)
{
    /**Common slider settings*/
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromFloatRGBA(0.0, 0.0, 0.0, 0.4));
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::whitesmoke.darker(0.5));
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromFloatRGBA(0.0, 0.0, 0.0, 0.0));
    slider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::fromFloatRGBA(1.0, 1.0, 1.0, 0.25));
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(1.0, 1.0, 1.0, 0.0));
    slider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colour::fromFloatRGBA(0.0, 0.0, 0.0, 0.0));
    slider.setDoubleClickReturnValue(true, 0.0);
    
    /**Specific slider settings*/
    lowPassSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lpfSliderId, lowPassSlider);
    lowPassSlider.setRange(3000, 20000, 1);
    lowPassSlider.setSkewFactorFromMidPoint(5000);
    lowPassSlider.setLookAndFeel(&faderSlider);
    
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);
    driveSlider.setRange(0.0, 16.0, 0.25);
    driveSlider.setLookAndFeel(&faderSlider);
    
    trimDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
    trimSlider.setRange(-20.0, 20.0, 0.25);
    trimSlider.setLookAndFeel(&faderSlider);
    
    /**Apparently this needs to be last or it won't show up*/
    slider.setComponentEffect(&sliderShadow);

}
