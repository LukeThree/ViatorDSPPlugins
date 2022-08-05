/*
  ==============================================================================

    Filters.cpp
    Created: 20 May 2021 11:22:01pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"

void GraphicEQAudioProcessor::updateFilter1(float gain)
{
    *filter1.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(lastSampleRate, 50.0f, getShelfQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter2(float gain, int bell)
{
    if (bell == 1){
        
        *filter2.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 63.0f, getPeakQ(gain), getGain(gain));
        
    } else {
        
        *filter2.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(lastSampleRate, 100.0f, getShelfQ(gain), getGain(gain));
    }
}

void GraphicEQAudioProcessor::updateFilter3(float gain)
{
    *filter3.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 125.0f, getPeakQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter4(float gain)
{
    *filter4.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 250.0f, getPeakQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter5(float gain)
{
    *filter5.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 500.0f, getPeakQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter6(float gain)
{
    *filter6.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 1000.0f, getPeakQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter7(float gain)
{
    *filter7.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 2000.0f, getPeakQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter8(float gain)
{
    *filter8.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 4000.0f, getPeakQ(gain), getGain(gain));
}

void GraphicEQAudioProcessor::updateFilter9(float gain, int bell)
{
    if (bell == 1){
        
        *filter9.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 8000.0f, getPeakQ(gain), getGain(gain));

    } else {
        
        *filter9.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 6000.0f, getShelfQ(gain), getGain(gain));
    }
}

void GraphicEQAudioProcessor::updateFilter10(float gain)
{
    *filter10.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 9000.0f, getShelfQ(gain), getGain(gain));
}

float GraphicEQAudioProcessor::getShelfQ(float value)
{
    return pow(10.0f, abs(value) / 60.0f) - 0.8f;
}

float GraphicEQAudioProcessor::getPeakQ(float value)
{
    
    if (value > 0.0f)
    {
        return pow(10.0f, abs(value) / 40.0f) - 0.9f;
        
    }
    
    else
    {
        return pow(10.0f, abs(value) / 20.0f) - 0.9f;
    }
}

float GraphicEQAudioProcessor::getGain(float value)
{
    return pow(10.0f, value / 20);
}
