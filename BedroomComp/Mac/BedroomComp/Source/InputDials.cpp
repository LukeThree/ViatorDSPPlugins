/*
  ==============================================================================

    InputDials.cpp
    Created: 20 Jun 2021 8:44:17pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setinputDialProps()
{
    inputDial.setRange(-66.0, 66.0, 0.25);
    inputDial.setTextValueSuffix(" dB");
    inputDial.setDoubleClickReturnValue(true, 0.0);
    
    hpfDialAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, hpfId, hpfDial);
    hpfDial.setRange(20.0, 350.0, 1.0);
    hpfDial.setSkewFactorFromMidPoint(100.0);
    hpfDial.setTextValueSuffix(" Hz");
    hpfDial.setDoubleClickReturnValue(true, 20.0);
    
    saturationDial.setRange(0.0, 100.0, 1.0);
    saturationDial.setTextValueSuffix(" %");
    saturationDial.setDoubleClickReturnValue(true, 0.0);
    saturationDial.setVisible(false);
    saturationDial.setEnabled(false);
    
    meterNeedleDial.setLookAndFeel(&customMeterDial);
    meterNeedleDial.setRange(-30.0, 30.0, 0.1);
    meterNeedleDial.setSkewFactorFromMidPoint(-2.0);
    meterNeedleDial.setValue(0.0);
    meterNeedleDial.setTextBoxIsEditable(false);
    meterNeedleDial.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::blue.withAlpha(0.0f));
    meterNeedleDial.toBack();
    meterNeedleDial.setEnabled(false);
}
