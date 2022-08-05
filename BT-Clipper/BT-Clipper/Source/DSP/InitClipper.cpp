/*
  ==============================================================================

    PrepareSaveClipper.cpp
    Created: 19 Jan 2022 6:11:33pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void BTClipperAudioProcessor::initClipper()
{
    clipperModule.setParameter(LV_Clipper::ParameterId::kDrive, *treeState.getRawParameterValue(driveID));
    clipperModule.setParameter(LV_Clipper::ParameterId::kCeiling, *treeState.getRawParameterValue(ceilingID));
    clipperModule.setParameter(LV_Clipper::ParameterId::kTrim, *treeState.getRawParameterValue(trimID));
    clipperModule.setParameter(LV_Clipper::ParameterId::kMix, *treeState.getRawParameterValue(mixID));
    clipperModule.setParameter(LV_Clipper::ParameterId::kBypass, *treeState.getRawParameterValue(clipperPowerID));
    
    switch (static_cast<int>(*treeState.getRawParameterValue(clipTypeID)))
    {
        case 0: clipperModule.setClipperType(LV_Clipper::ClipperTypeId::kHardClipper); break;
        case 1: clipperModule.setClipperType(LV_Clipper::ClipperTypeId::kSoftClipper); break;
        case 2: clipperModule.setClipperType(LV_Clipper::ClipperTypeId::kAnalogClipper); break;
    }
}
