/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BedroomCompAudioProcessorEditor::BedroomCompAudioProcessorEditor (BedroomCompAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    AudioProcessorEditor::setResizable(false, false);
    setSize (600, 400);
    
    //int timerRate = static_cast<int>(audioProcessor.getSampleRate() / 5.0 / audioProcessor.getBufferSize());
    
    startTimerHz(24);
    
    initDials();
    setinputDialProps();
    setEQDialProps();
    setCompressorDials();

    initButtons();
    setPhaseToggleProps();
    setSaturationTypeToggleProps();
    
    initBorders();
    initLabels();

    setOversampleMenuProps();

}

BedroomCompAudioProcessorEditor::~BedroomCompAudioProcessorEditor()
{
    for (auto i {0}; i < dials.size(); i++)
    {
        dials[i]->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void BedroomCompAudioProcessorEditor::paint (juce::Graphics& g)
{
    setBackgroundProps(g);
    setHeaderProps(g);
}

void BedroomCompAudioProcessorEditor::resized()
{
    const auto leftMargin = AudioProcessorEditor::getWidth() / 20;

    const auto topMargin = AudioProcessorEditor::getHeight() / 4;
    
    const auto dialSize = AudioProcessorEditor::getWidth() / 6;

    // Input Output =================================================================================================================================
    
    compGainDial.setBounds(leftMargin, topMargin * 1.2, dialSize, dialSize);
    hpfDial.setBounds(leftMargin, compGainDial.getY() + compGainDial.getHeight() * 1.5, dialSize, dialSize);
    outputBorder.setBounds(compGainDial.getX() / 2, compGainDial.getY() * 0.6, compGainDial.getX() + compGainDial.getWidth(), compGainDial.getHeight() * 3.2);

    threshDial.setBounds(leftMargin * 12.5, compGainDial.getY(), dialSize, dialSize);

    ratioDial.setBounds(threshDial.getX() + threshDial.getWidth(), threshDial.getY(), dialSize, dialSize);
    ratioValueLabel.setBounds(ratioDial.getX(), ratioDial.getY() * 1.64, ratioDial.getWidth(), 16);
    
    speedDial.setBounds(threshDial.getX(), hpfDial.getY(), dialSize, dialSize);
    blendDial.setBounds(ratioDial.getX(), hpfDial.getY(), dialSize, dialSize);
    
    compressorToggle.setBounds(leftMargin * 6.5, hpfDial.getY() * 0.85, dialSize / 1.5, dialSize / 3);
    hpfSideChainToggle.setBounds(compressorToggle.getX() + compressorToggle.getWidth() * 1.5, compressorToggle.getY(), compressorToggle.getWidth(), compressorToggle.getHeight());

    meterNeedleDial.setBounds(AudioProcessorEditor::getWidth() * 0.12, AudioProcessorEditor::getHeight() * .18, dialSize * 4, dialSize * 4);
    
    compressorBorder.setBounds(outputBorder.getX() + outputBorder.getWidth() * 1.1, outputBorder.getY(), dialSize * 4.3, outputBorder.getHeight());


}

float BedroomCompAudioProcessorEditor::float_one_point_round(float value)
{
        return ((float)((int)(value * 10))) / 10;
}
