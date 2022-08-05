/*
  ==============================================================================

    ChaosModule.cpp
    Created: 8 May 2021 4:24:19pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

float OverdrivePedalAudioProcessor::getRandomNumber(float timeElapsed)
{
    
    float rawNumber = (float)std::rand() / RAND_MAX;

    return std::ceil(rawNumber * 100.0f) / 100.0f + timeElapsed;
    
}

float OverdrivePedalAudioProcessor::calculateDeJongMap(float x)
{
    
    float white = whiteNoise.nextFloat() - 0.5f;
        
    float b0 = 0.99765 + white * 0.0990460;
    float b1 = 0.96300 + white * 0.2965164;
    float b2 = 0.57000 + white * 1.0526913;
    float pink = b0 + b1 + b2 + white * 0.1848;
    
    float a = 4.1f;
    float b = -4.8f;
    float output;
    
    output = sin(sin(a * x) * x) - cos(sin(b * x) * x) + pink;
    
    x = output * 0.01f;
    
    return output;
}
