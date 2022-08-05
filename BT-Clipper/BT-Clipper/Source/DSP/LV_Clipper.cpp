/*
  ==============================================================================

    LV_Clipper.cpp
    Created: 12 Jan 2022 3:55:22pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_Clipper.h"

void LV_Clipper::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    drive.reset(sampleRate, 0.02);
    ceiling.reset(sampleRate, 0.02);
    trim.reset(sampleRate, 0.02);
    mix.reset(sampleRate, 0.02);
}

void LV_Clipper::processBlock(juce::dsp::AudioBlock<float>& block)
{
    
    if (bypassModule) return;
    
    for (auto sample {0}; sample < block.getNumSamples(); sample++)
    {
        for (auto ch {0}; ch < block.getNumChannels(); ch++)
        {
            float* data = block.getChannelPointer(ch);
            
            // Raw input
            drySignal = data[sample];
            const auto newInput = data[sample] * juce::Decibels::decibelsToGain(ceiling.getNextValue() * -1.0);
            
            switch (clipperType)
            {
                case ClipperTypeId::kHardClipper:
                {
                    clippedSignal = returnHardClip(newInput * logScale(drive.getNextValue()), ch);
                    clippedSignal *= juce::Decibels::decibelsToGain(ceiling.getNextValue());
                    clippedSignal *= juce::Decibels::decibelsToGain(trim.getNextValue());
                    blendSignal = (1.0 - mix.getNextValue()) * drySignal + mix.getNextValue() * clippedSignal;
                    data[sample] = blendSignal;
                    break;
                }
                    
                case ClipperTypeId::kSoftClipper:
                {
                    clippedSignal = returnSoftClip(newInput * logScale(juce::jmap(drive.getNextValue(), 0.0f, 24.0f, -1.0f, 4.0f)));
                    clippedSignal *= juce::Decibels::decibelsToGain(ceiling.getNextValue());
                    clippedSignal *= juce::Decibels::decibelsToGain(trim.getNextValue());
                    blendSignal = (1.0 - mix.getNextValue()) * drySignal + mix.getNextValue() * clippedSignal;
                    data[sample] = blendSignal;
                    break;
                }
                    
                    
                case ClipperTypeId::kAnalogClipper:
                {
                    clippedSignal = returnAnalogClip(newInput * logScale(drive.getNextValue()), ch);
                    clippedSignal *= juce::Decibels::decibelsToGain(ceiling.getNextValue());
                    clippedSignal *= juce::Decibels::decibelsToGain(trim.getNextValue());
                    blendSignal = (1.0 - mix.getNextValue()) * drySignal + mix.getNextValue() * clippedSignal;
                    data[sample] = blendSignal;
                    break;
                }
            }
        }
    }
}

float LV_Clipper::returnSoftClip(const float input)
{
    return softCoeffA * input + softCoeffB * std::pow(input, 3);
}

float LV_Clipper::returnHardClip(const float input, const int channel)
{
    auto newInput = piDivisor * std::atan(input) * juce::Decibels::decibelsToGain(6.0);
    
    if (std::abs(newInput) > 1)
    {
        newInput *= 1 / std::abs(newInput);
    }
    
    return newInput;
}

float LV_Clipper::returnAnalogClip(const float input, const int channel)
{
    auto diode = juce::Decibels::decibelsToGain(6.5) * (std::exp(analogCoeffA * input / (analogCoeffB * analogCoeffC)) - 1.0);
    
    if (std::abs(diode) > 1.0)
    {
        diode *= 1.0 / std::abs(diode);
    }
    
    return diode;
}

void LV_Clipper::setClipperType(ClipperTypeId newType)
{
    switch (newType)
    {
        case LV_Clipper::ClipperTypeId::kHardClipper:
        {
            clipperType = ClipperTypeId::kHardClipper;
            break;
        }
            
        case LV_Clipper::ClipperTypeId::kSoftClipper:
        {
            clipperType = ClipperTypeId::kSoftClipper;
            break;
        }
            
            
        case LV_Clipper::ClipperTypeId::kAnalogClipper:
        {
            clipperType = ClipperTypeId::kAnalogClipper;
            break;
        }
    }
}

void LV_Clipper::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case LV_Clipper::ParameterId::kDrive:
        {
            drive.setTargetValue(parameterValue);
            break;
        }
            
        case LV_Clipper::ParameterId::kCeiling:
        {
            ceiling.setTargetValue(parameterValue);
            break;
        }
            
        case LV_Clipper::ParameterId::kMix:
        {
            const auto newMix = juce::jmap(parameterValue, 0.0f, 100.0f, 0.0f, 1.0f);
            mix.setTargetValue(newMix);
            break;
        }
            
        case LV_Clipper::ParameterId::kTrim:
        {
            trim.setTargetValue(parameterValue);
            break;
        }
            
        case LV_Clipper::ParameterId::kBypass:
        {
            bypassModule = parameterValue;
            break;
        }
    }
}

float LV_Clipper::logScale(float value, float scalar) const
{
    return std::pow(10.0f, value * scalar);
}
