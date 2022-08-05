/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViatorDCDistortionAudioProcessorEditor::ViatorDCDistortionAudioProcessorEditor (ViatorDCDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    int leftMargin = 24;
    int topMargin = 24;
    
    sliders.reserve(3);
    sliders = {
        &inputSlider, &driveSlider, &trimSlider
    };
    
    tracks.reserve(3);
    tracks = {
        &inputSliderTrack, &driveSliderTrack, &trimSliderTrack
    };
    
    labels.reserve(3);
    labels = {
            &inputSliderLabel, &driveSliderLabel, &trimSliderLabel
    };
    
    labelTexts.reserve(3);
    labelTexts = {
        inputSliderLabelText, driveSliderLabelText, trimSliderLabelText
    };
    
    
    //For some reason, the interval works on the actually slider movement, but not the number that gets sent to the text box.
    //You need to use the text box of the non-track slider.
    for (size_t i {0}; i < sliders.size(); i++) {
        addAndMakeVisible(sliders[i]);
        //sliders[i]->addListener(this);
        sliders[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 256, 32);
        sliders[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        sliders[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        sliders[i]->setLookAndFeel(&otherLookAndFeel);
        
        //Slider Tracks
        addAndMakeVisible(tracks[i]);
        tracks[i]->addListener(this);
        tracks[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        tracks[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 128, 24);
        tracks[i]->setDoubleClickReturnValue(true, 0);
        tracks[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
        tracks[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        tracks[i]->setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
        tracks[i]->setColour(0x1001312, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
        tracks[i]->setColour(0x1001311, juce::Colour::fromFloatRGBA(.2, .77, 1, 0));
        tracks[i]->setBounds(leftMargin + 8, leftMargin + 4, 130, 130);
        
        if (sliders[i] == &inputSlider){
            sliders[i]->setRange(-24, 24, 0.25);
            tracks[i]->setRange(-24, 24, 0.25);
            tracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0);
            inputSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputSliderId, inputSliderTrack);
            
        } else if (sliders[i] == &driveSlider){
            sliders[i]->setRange(0, 24, 0.25);
            tracks[i]->setRange(0, 24, 0.25);
            tracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0);
            driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSliderTrack);

        } else if (sliders[i] == &trimSlider){
            sliders[i]->setRange(-24, 24, 0.25);
            tracks[i]->setRange(-24, 24, 0.25);
            tracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0);
            trimSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSliderTrack);
    }
        
        //Labels
        addAndMakeVisible(labels[i]);
        labels[i]->setText(labelTexts[i], juce::dontSendNotification);
        labels[i]->attachToComponent(sliders[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
        labels[i]->setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        
        if (sliders[i] == &inputSlider){
            sliders[i]->setBounds(leftMargin, topMargin + 32, 145, 145);
            tracks[i]->setBounds(leftMargin + 6, topMargin + 34, 133, 133);
        } else {
            sliders[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth(), topMargin + 32, 145, 145);
            tracks[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth() + 6, topMargin + 34, 133, 133);
        }
}
    
    setSize (500, 273);
}

ViatorDCDistortionAudioProcessorEditor::~ViatorDCDistortionAudioProcessorEditor()
{
}

//==============================================================================
void ViatorDCDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromFloatRGBA(0.14f, 0.16f, 0.2f, 1.0));

}

void ViatorDCDistortionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ViatorDCDistortionAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    for (size_t i {0}; i < sliders.size(); i++) {
        if (slider == tracks[i]){
            sliders[i]->setValue(tracks[i]->getValue());
        }
    }
}
