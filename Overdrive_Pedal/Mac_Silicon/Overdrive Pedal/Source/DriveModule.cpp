/*
  ==============================================================================

    DriveModule.cpp
    Created: 8 May 2021 3:40:17pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


void OverdrivePedalAudioProcessor::updateDrive(const float &drive){
    
    mDrive = drive + 4;
    
}

void OverdrivePedalAudioProcessor::prepareDrive()
{
    updateDrive(*treeState.getRawParameterValue(driveSliderId));

}

float OverdrivePedalAudioProcessor::getTanhOf(const float input)
{
    return tanh(input);
}

float OverdrivePedalAudioProcessor::convertToDrive(const float drive)
{
    return pow(10, drive / 20);
}
