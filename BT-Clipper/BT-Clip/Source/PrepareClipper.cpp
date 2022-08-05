/*
  ==============================================================================

    PrepareClipper.cpp
    Created: 13 Oct 2021 7:34:07pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void BTClipAudioProcessor::prepare_clipper(juce::dsp::ProcessSpec spec)
{
    m_ClippersModule.reset();
    m_ClippersModule.prepare(spec);
    m_ClippersModule.set_clipping_type(LV_Clippers::ClippingType::kHardClip);
    m_ClippersModule.setParameter(LV_Clippers::ParameterId::kDrive, m_Drive);
}
