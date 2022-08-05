/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionMeetupAudioProcessorEditor::DistortionMeetupAudioProcessorEditor (DistortionMeetupAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), controls(audioProcessor.treeState)
{
    addAndMakeVisible(controls);
    
    bandOneSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    bandOneSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 64, 32);
    bandOneSlider.setDoubleClickReturnValue(true, 1.0);
    bandOneSlider.setTextValueSuffix("bruh");
    bandOneSlider.setRange(0.1, 2.0, 0.05);
    bandOneSlider.setValue(1.0);
    addAndMakeVisible(bandOneSlider);
    
    bandOneSlider.onValueChange = [this]()
    {
        audioProcessor.bandOneCutoffMult = bandOneSlider.getValue();
    };
    
    bandTwoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    bandTwoSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 64, 32);
    bandTwoSlider.setDoubleClickReturnValue(true, 1.0);
    bandTwoSlider.setTextValueSuffix("bruh");
    bandTwoSlider.setRange(0.1, 2.0, 0.05);
    bandTwoSlider.setValue(1.0);
    addAndMakeVisible(bandTwoSlider);
    
    bandTwoSlider.onValueChange = [this]()
    {
        audioProcessor.bandTwoCutoffMult = bandTwoSlider.getValue();
    };
    
    setSize (804, 500);
}

DistortionMeetupAudioProcessorEditor::~DistortionMeetupAudioProcessorEditor()
{
}

//==============================================================================
void DistortionMeetupAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black.brighter(0.1f));

    g.setColour (juce::Colours::whitesmoke.withAlpha(0.25f));
    g.setFont (24.0f);
    g.drawFittedText ("ADPTR Core Waveshaper", 8, 8, getWidth() * 0.4, getHeight() * 0.25, juce::Justification::topLeft, 1);
}

void DistortionMeetupAudioProcessorEditor::resized()
{
    // Helpful vars
    auto width = getWidth();
    auto height = getHeight();
    
    controls.setBoundsConstraints(width, height);
    controls.setBounds(0, 0, getWidth(), getHeight() * 0.75);
    
    bandOneSlider.setBounds(64, controls.getY() + controls.getHeight() + 100, 256, 32);
    bandTwoSlider.setBounds(bandOneSlider.getX() + bandOneSlider.getWidth(), bandOneSlider.getY(), 256, 32);
}
