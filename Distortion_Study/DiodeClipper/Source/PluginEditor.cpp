/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DiodeClipperAudioProcessorEditor::DiodeClipperAudioProcessorEditor (DiodeClipperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    int leftMargin = 24;
    int topMargin = 24;
    
    sliders.reserve(3);
    sliders = {
        &thermalVoltageSlider, &emissionCoefficientSlider, &saturationCurrentSlider
    };
    
    tracks.reserve(3);
    tracks = {
        &thermalVoltageSliderTrack, &emissionCoefficientSliderTrack, &saturationCurrentSliderTrack
    };
    
    labels.reserve(3);
    labels = {
            &thermalVoltageLabel, &emissionCoefficientLabel, &saturationCurrentLabel
    };
    
    labelTexts.reserve(3);
    labelTexts = {
        thermalVoltageText, emissionCoefficientText, saturationCurrentText
    };
    
    for (size_t i {0}; i < sliders.size(); i++) {
        addAndMakeVisible(sliders[i]);
        //sliders[i]->addListener(this);
        sliders[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 256, 32);
        sliders[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        sliders[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        sliders[i]->setLookAndFeel(&otherLookAndFeel);
        
        //Slider Tracks
//        addAndMakeVisible(tracks[i]);
//        tracks[i]->addListener(this);
//        tracks[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//        tracks[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 128, 24);
//        tracks[i]->setDoubleClickReturnValue(true, 0);
//        tracks[i]->setNumDecimalPlacesToDisplay(1);
//        tracks[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
//        tracks[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
//        tracks[i]->setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
//        tracks[i]->setColour(0x1001312, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
//        tracks[i]->setColour(0x1001311, juce::Colour::fromFloatRGBA(.2, .77, 1, 0));
//        tracks[i]->setBounds(leftMargin + 8, leftMargin + 4, 130, 130);
        
        if (sliders[i] == &thermalVoltageSlider){
            sliders[i]->setRange(0.001f, 0.09f, 0.001f);
            //tracks[i]->setRange(0.001f, 0.09f, 0.001f);
           // tracks[i]->setTextValueSuffix(" mV");
            sliders[i]->setTextValueSuffix(" mV");
            sliders[i]->setDoubleClickReturnValue(true, 0.0253f);
            thermalVoltageSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, thermalVoltageSliderId, thermalVoltageSliderTrack);
            
        } else if (sliders[i] == &emissionCoefficientSlider){
            sliders[i]->setRange(1.0f, 2.0f, 0.01f);
            //tracks[i]->setRange(1.0f, 2.0f, 0.01f);
            //tracks[i]->setTextValueSuffix(" n");
            sliders[i]->setTextValueSuffix(" n");
            sliders[i]->setDoubleClickReturnValue(true, 1.68f);
            emissionCoefficientSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, emissionCoefficientSliderId, emissionCoefficientSliderTrack);

        } else if (sliders[i] == &saturationCurrentSlider){
            
            //tracks[i]->setTextValueSuffix(" Is");
            sliders[i]->setTextValueSuffix(" Is");
            sliders[i]->setDoubleClickReturnValue(true, 0.105f);
            saturationCurrentSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, saturationCurrentSliderId, saturationCurrentSliderTrack);
            sliders[i]->setRange(0.001f, 1.0f, 0.001f);
            //tracks[i]->setRange(0.001f, 1.0f, 0.001f);
    }
        
        //Labels
        addAndMakeVisible(labels[i]);
        labels[i]->setText(labelTexts[i], juce::dontSendNotification);
        labels[i]->attachToComponent(sliders[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
        labels[i]->setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        
        if (sliders[i] == &thermalVoltageSlider){
            sliders[i]->setBounds(leftMargin, topMargin + 32, 145, 145);
            //tracks[i]->setBounds(leftMargin + 6, topMargin + 34, 133, 133);
        } else {
            sliders[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth(), topMargin + 32, 145, 145);
            //tracks[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth() + 6, topMargin + 34, 133, 133);
        }
}
    
    setSize (500, 273);
}

DiodeClipperAudioProcessorEditor::~DiodeClipperAudioProcessorEditor()
{
}

//==============================================================================
void DiodeClipperAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromFloatRGBA(0.14f, 0.16f, 0.2f, 1.0));

}

void DiodeClipperAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void DiodeClipperAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    for (size_t i {0}; i < sliders.size(); i++) {
        if (slider == tracks[i]){
            sliders[i]->setValue(tracks[i]->getValue());
        }
    }
}
