/*
  ==============================================================================

    Parameters.cpp
    Created: 16 Apr 2022 1:45:51pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void MultiQAudioProcessor::updateCommonParameters()
{
    osToggle = static_cast<bool>(treeState.getRawParameterValue(qualityID)->load());
    phaseToggle = treeState.getRawParameterValue(phaseID)->load();
    
    hpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, treeState.getRawParameterValue(highpassID)->load());
    lpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, treeState.getRawParameterValue(lowpassID)->load());

    switch (static_cast<int>(treeState.getRawParameterValue(msID)->load()))
    {
        case 0:
        {
            hpFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            lpFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            break;
        }
            
        case 1:
        {
            hpFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            lpFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            break;
        }
            
        case 2:
        {
            hpFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            lpFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            break;
        }
    }
    
    gainModule.setGainDecibels(treeState.getRawParameterValue(trimID)->load());
}
