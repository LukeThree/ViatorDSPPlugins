/*
  ==============================================================================

    PrepareDSP.cpp
    Created: 30 Nov 2021 6:37:15pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginProcessor.h"

void PhonemodelAudioProcessor::prepareDSP(juce::dsp::ProcessSpec &spec)
{
    /** Filters */
    lowFilter.prepare(spec);
    highFilter.prepare(spec);
    midToneFilter.prepare(spec);
    lowCompensateFilter.prepare(spec);
    lowShelfFilter.prepare(spec);
    highShelfFilter.prepare(spec);
    
    /** Rectifiers */
    inputRectifierModule.prepare(spec);
    outputRectifierModule.prepare(spec);
}
