/*
  ==============================================================================

    EQDials.cpp
    Created: 20 Jun 2021 4:05:47pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BedroomCompAudioProcessorEditor::setEQDialProps()
{
    lowGainDial.setRange(-15.0, 15.0, 0.25);
    lowGainDial.setTextValueSuffix(" dB");
    lowGainDial.setDoubleClickReturnValue(true, 0.0);
    
    midGainDial.setRange(-15.0, 15.0, 0.25);
    midGainDial.setTextValueSuffix(" dB");
    midGainDial.setDoubleClickReturnValue(true, 0.0);
    
    highGainDial.setRange(-15.0, 15.0, 0.25);
    highGainDial.setTextValueSuffix(" dB");
    highGainDial.setDoubleClickReturnValue(true, 0.0);
    
    lowFreqDial.setRange(35.0, 220.0, 1.0);
    lowFreqDial.setTextValueSuffix(" Hz");
    lowFreqDial.setDoubleClickReturnValue(true, 35.0);
    
    midFreqDial.setRange(220.0, 7500.0, 1.0);
    midFreqDial.setSkewFactorFromMidPoint(1000.0);
    midFreqDial.setTextValueSuffix(" Hz");
    midFreqDial.setDoubleClickReturnValue(true, 220.0);
}
