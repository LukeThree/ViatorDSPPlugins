/*
  ==============================================================================

    FiltersModule.cpp
    Created: 8 May 2021 3:23:11pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


void OverdrivePedalAudioProcessor::updateHSFilter(const float &gain){
    
    *hsFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 1000.0f, 1.0f, gain);
    
}

void OverdrivePedalAudioProcessor::prepareFilters(juce::dsp::ProcessSpec spec)
{
    lpFilter.prepare(spec);
    lpFilter.reset();
    lpFilter.setEnabled(true);
    lpFilter.setMode(juce::dsp::LadderFilterMode::LPF12);
    lpFilter.setCutoffFrequencyHz(lpFilterCutoff);
    lpFilter.setResonance(0.0f);
    lpFilter.setDrive(1.0f);
    
    hpFilter.prepare(spec);
    hpFilter.reset();
    hpFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    hpFilter.setCutoffFrequency(175.0f);
    hpFilter.setResonance(1.0f);
    
    hsFilter.prepare(spec);
    hsFilter.reset();
    updateHSFilter(pow(10, *treeState.getRawParameterValue(toneSliderId) / 20));

}
