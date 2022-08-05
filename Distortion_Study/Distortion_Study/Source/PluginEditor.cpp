/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string>

//==============================================================================
Distortion_StudyAudioProcessorEditor::Distortion_StudyAudioProcessorEditor (Distortion_StudyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    int leftMargin = 24;
    int topMargin = 24;
    
    sliders.reserve(3);
    sliders = {
        &inputGainSlider, &biasSlider, &outputGainSlider
    };
    
    tracks.reserve(3);
    tracks = {
        &inputGainSliderTrack, &biasSliderTrack, &outputGainSliderTrack
    };
    
    labels.reserve(3);
    labels = {
            &inputLabel, &biasLabel, &outputLabel
    };
    
    labelTexts.reserve(3);
    labelTexts = {
        inputText, biasText, outputText
    };
    
    for (size_t i {0}; i < sliders.size(); i++) {
        addAndMakeVisible(sliders[i]);
        sliders[i]->addListener(this);
        sliders[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 256, 32);
        sliders[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        sliders[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        sliders[i]->setLookAndFeel(&otherLookAndFeel);
        
        //Slider Tracks
        addAndMakeVisible(tracks[i]);
        tracks[i]->addListener(this);
        tracks[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        tracks[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 128, 24);
        tracks[i]->setDoubleClickReturnValue(true, 0);
        tracks[i]->setNumDecimalPlacesToDisplay(1);
        tracks[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
        tracks[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        tracks[i]->setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
        tracks[i]->setColour(0x1001312, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
        tracks[i]->setColour(0x1001311, juce::Colour::fromFloatRGBA(.2, .77, 1, 0));
        tracks[i]->setBounds(leftMargin + 8, leftMargin + 4, 130, 130);
        
        if (sliders[i] == &inputGainSlider){
            sliders[i]->setRange(-48.0f, 48.0f, 0.25);
            tracks[i]->setRange(-48.0f, 48.0f, 0.25);
            tracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0.0f);
            inputGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputGainSliderId, inputGainSliderTrack);
            
        } else if (sliders[i] == &biasSlider){
            sliders[i]->setRange(-1.0f, 1.0f, 0.01);
            tracks[i]->setRange(-1.0f, 1.0f, 0.01);
            tracks[i]->setTextValueSuffix(" V");
            sliders[i]->setTextValueSuffix(" V");
            sliders[i]->setDoubleClickReturnValue(true, 0);
            biasSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, biasSliderId, biasSliderTrack);

        } else if (sliders[i] == &outputGainSlider){
            sliders[i]->setRange(-48.0f, 48.0f, 0.25);
            tracks[i]->setRange(-48.0f, 48.0f, 0.25);
            tracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 100);
            outputGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputGainSliderId, outputGainSliderTrack);
    }
        
        //Labels
        addAndMakeVisible(labels[i]);
        labels[i]->setText(labelTexts[i], juce::dontSendNotification);
        labels[i]->attachToComponent(sliders[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
        labels[i]->setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        
        if (sliders[i] == &inputGainSlider){
            sliders[i]->setBounds(leftMargin, topMargin + 32, 145, 145);
            tracks[i]->setBounds(leftMargin + 6, topMargin + 34, 133, 133);
        } else {
            sliders[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth(), topMargin + 32, 145, 145);
            tracks[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth() + 6, topMargin + 34, 133, 133);
        }
}
    
    setSize (500, 273);
}

Distortion_StudyAudioProcessorEditor::~Distortion_StudyAudioProcessorEditor()
{
}

//==============================================================================
void Distortion_StudyAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromFloatRGBA(0.14f, 0.16f, 0.2f, 1.0));

}

void Distortion_StudyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Distortion_StudyAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    for (size_t i {0}; i < sliders.size(); i++) {
        if (slider == tracks[i]){
            sliders[i]->setValue(tracks[i]->getValue());
        }
    }
}
