/*
  ==============================================================================

    LV_SplitDistortion.cpp
    Created: 7 Nov 2021 3:30:43pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LV_SplitDistortion.h"

LV_SplitDistortion::LV_SplitDistortion()
: preamp(0.0), curve(0.0), wet(0.0), cutoff(2500.0), trim(0.0), sampleRate(44100.0)
,  bypassModule(false), clipInput(false), bandMode(BandMode::kAllBands), modelMode(ModelMode::kInflator)
{
}

void LV_SplitDistortion::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    prepareTopBandFilter(spec);
    
    outputModule.reset();
    outputModule.prepare(spec);
    outputModule.setRampDurationSeconds(0.02);
}

float LV_SplitDistortion::getSineClip(float input)
{
    float gainReduce = 1.0 - 0.5 * preamp;
    
    return std::sin(input * juce::Decibels::decibelsToGain(preamp)) * juce::Decibels::decibelsToGain(gainReduce);
}

float LV_SplitDistortion::getFoldClip(float input)
{
    float x = input * juce::Decibels::decibelsToGain(preamp);
    
    bool isNegative = x < 0.0 ? true : false;
    
    float absx = std::fabs (x);
    
    if (absx > 1.0)
    {
        int xint = absx;
        float xrem = absx - xint;
        
        bool odd = absx;
        
        if (odd)
            x = 1.0 - xrem;
        else
            x = 0.0 + xrem; //(1.0 - xrem);
        
        if (isNegative)
            x = -x;
    }
    
    return x;
}

float LV_SplitDistortion::getVariClip(float input)
{
    const float newPreamp = juce::jmap(preamp, 0.0f, 12.0f, 0.0f, 50.0f);
    auto this_gain = juce::Decibels::decibelsToGain(newPreamp);
    float x = input;
    
    float sign = x > 0.0 ? 1.0 : -1.0;
    float abs_x = std::fabs(x);
    
    // gain
    abs_x *= this_gain;
    float y = sign * std::pow(std::atan(std::pow (abs_x, 4.35)), 0.23);
                  
    return y * juce::Decibels::decibelsToGain(-6.0);
}

float LV_SplitDistortion::getSoftClip(float input)
{
    float x = input * juce::Decibels::decibelsToGain(preamp);
    float abs_x = std::fabs(x);
        
    if (abs_x > 0.5)
        x = (abs_x - 0.25) / x;
        
    return x;
}

float LV_SplitDistortion::getTubeDistortion(float input)
{
    const float newPreamp = juce::jmap(preamp, 0.0f, 12.0f, 0.0f, 30.0f);
    auto this_gain = input * juce::Decibels::decibelsToGain(newPreamp);
    
    auto x = this_gain;
    auto a = 1.0 + std::exp(std::sqrt(std::abs (x)) * -0.75);
    auto ex = std::exp(x);

    auto tube = (ex - std::exp(-x * a)) * (ex / (ex * ex + 1));
    
    return 2/3.14 * std::atan(tube);
}

float LV_SplitDistortion::getSaturation(float input)
{
    const float newPreamp = juce::jmap(preamp, 0.0f, 12.0f, 0.0f, 6.0f);
    float dryMix = wet;
    
    float next_drive = input * juce::Decibels::decibelsToGain(newPreamp);
    
    if (next_drive < 0.5)
    {
        dryMix = 2.0 * (0.5 - next_drive);
        next_drive = 0.5;
    }
    
    const float x_n = input * next_drive;
    
    const auto saturation = 2.0 * x_n / (1.0 + x_n * x_n);
    
    return saturation * dryMix + (input * (1.0 - dryMix));
}

float LV_SplitDistortion::getInflator(float input)
{
    // Curve mixes
    auto positiveCurveMix = juce::jmap(curve, 0.0f, 50.0f, 0.0f, 0.25f);
    auto negativeCurveMix = juce::jmap(curve, 0.0f, -50.0f, 0.0f, 0.5f);
    
    // Clip Input
    if (clipInput)
    {
        input = hardClip(input);
    }
    
    // Input variables
    auto inflatorinput = input * juce::Decibels::decibelsToGain(preamp);
    auto inflatorPlusCurveInput = input * juce::Decibels::decibelsToGain(((preamp * 0.5) + 11.5));
    auto inflatorMinusCurveInput = input * juce::Decibels::decibelsToGain(((preamp * 0.5) + 10.0));
    
    // Basic inflator shape
    auto inflator = 1.5 * inflatorinput - 0.5 * std::pow(inflatorinput, 3);
    
    // Inflator with positive curve
    auto inflatorPlusCurve = 0.01746 * std::pow(inflatorPlusCurveInput, 3) - 0.7695 * inflatorPlusCurveInput;
    
    // Inflator with negative curve
    auto inflatorMinusCurve = 0.00618 * std::pow(inflatorMinusCurveInput, 3) - 0.2217 * inflatorMinusCurveInput;
    inflatorMinusCurve *= juce::Decibels::decibelsToGain(3.6);
    
    // Output mix
    if (curve >= 0.0)
    {
        float plusOutput = (1.0 - positiveCurveMix) * (inflator * -1.0) + juce::dsp::FastMathApproximations::tanh(inflatorPlusCurve) * positiveCurveMix;
        return (1.0 - wet) * input * -1.0 + plusOutput * wet;
    }

    else
    {
        float minusOutput = (1.0 - negativeCurveMix) * (inflator * -1.0) + juce::dsp::FastMathApproximations::tan(inflatorMinusCurve) * negativeCurveMix;
        return (1.0 - wet) * input * -1.0 + minusOutput * wet;
    }
}

float LV_SplitDistortion::hardClip(float sample)
{
    if (std::abs(sample) > 1.0)
    {
        sample *= 1.0 / std::abs(sample);
    }
    
    return sample;
}

float LV_SplitDistortion::sm_map(float value, float min, float max, float curve)
{
    if (value < 0.0)
        value = 0.0;
    else if (value > 1.0)
        value = 1.0;
    
    double range = max - min;
    
    if (fabs(curve) < 0.001)
        return value * range + min;
    
    double grow = exp(curve);
    double a = (max - min) / (1.0 - grow);
    double b = min + a;
    
    return b - (a * pow(grow, value));
}

void LV_SplitDistortion::processBlock(juce::dsp::AudioBlock<float>& block)
{
    auto processingContext = juce::dsp::ProcessContextReplacing<float>(block);
    
    for (auto sample {0}; sample < block.getNumSamples(); sample++)
    {
        for (auto ch {0}; ch < block.getNumChannels(); ch++)
        {
            float* data = block.getChannelPointer(ch);
            
            // Raw input
            const auto input = data[sample];
            
            auto lowBandSignal = bandOneFilter.processSample(input, ch);
            auto highBandSignal = input - lowBandSignal;
            
            // Distort either the top or bottom band
            switch (bandMode)
            {
                case LV_SplitDistortion::BandMode::kAllBands:
                {
                    switch (modelMode)
                    {
                        case LV_SplitDistortion::ModelMode::kInflator: data[sample] = getInflator(input); break;
                        case LV_SplitDistortion::ModelMode::kSaturation: data[sample] = getSaturation(input); break;
                        case LV_SplitDistortion::ModelMode::kTube: data[sample] = getTubeDistortion(input); break;
                        case LV_SplitDistortion::ModelMode::kSoft: data[sample] = getSoftClip(input); break;
                        case LV_SplitDistortion::ModelMode::kFold: data[sample] = getFoldClip(input); break;
                        case LV_SplitDistortion::ModelMode::kVari: data[sample] = getVariClip(input); break;
                        case LV_SplitDistortion::ModelMode::kSine: data[sample] = getSineClip(input); break;
                    }
                    
                    data[sample] = hardClip(data[sample]);
                    break;
                }
                    
                case LV_SplitDistortion::BandMode::kLowBand:
                {
                    switch (modelMode)
                    {
                        case LV_SplitDistortion::ModelMode::kInflator: data[sample] = getInflator(lowBandSignal) + highBandSignal; break;
                        case LV_SplitDistortion::ModelMode::kSaturation: data[sample] = getSaturation(lowBandSignal) + highBandSignal; break;
                        case LV_SplitDistortion::ModelMode::kTube: data[sample] = getTubeDistortion(input); break;
                        case LV_SplitDistortion::ModelMode::kSoft: data[sample] = getSoftClip(input); break;
                        case LV_SplitDistortion::ModelMode::kFold: data[sample] = getFoldClip(input); break;
                        case LV_SplitDistortion::ModelMode::kVari: data[sample] = getVariClip(input); break;
                        case LV_SplitDistortion::ModelMode::kSine: data[sample] = getSineClip(input); break;
                    }
                    
                    data[sample] = hardClip(data[sample]);
                    break;
                }
                    
                case LV_SplitDistortion::BandMode::kHighBand:
                {
                    switch (modelMode)
                    {
                        case LV_SplitDistortion::ModelMode::kInflator: data[sample] = getInflator(highBandSignal) + lowBandSignal; break;
                        case LV_SplitDistortion::ModelMode::kSaturation: data[sample] = getSaturation(highBandSignal) + lowBandSignal; break;
                        case LV_SplitDistortion::ModelMode::kTube: data[sample] = getTubeDistortion(input); break;
                        case LV_SplitDistortion::ModelMode::kSoft: data[sample] = getSoftClip(input); break;
                        case LV_SplitDistortion::ModelMode::kFold: data[sample] = getFoldClip(input); break;
                        case LV_SplitDistortion::ModelMode::kVari: data[sample] = getVariClip(input); break;
                        case LV_SplitDistortion::ModelMode::kSine: data[sample] = getSineClip(input); break;
                    }
                    
                    data[sample] = hardClip(data[sample]);
                    break;
                }
            }
        }
    }
    
    outputModule.process(processingContext);
}

void LV_SplitDistortion::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case LV_SplitDistortion::ParameterId::kPreamp:
        {
            assertRange(parameterValue, -24.0, 24.0f);
            preamp = parameterValue;
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kCurve:
        {
            curve = parameterValue;
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kWet:
        {
            assertRange(parameterValue, 0.0f, 100.0f);
            wet = juce::jmap(parameterValue, 0.0f, 100.0f, 0.0f, 1.0f);
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kTrim:
        {
            assertRange(parameterValue, -48.0, 48.0f);
            trim = parameterValue;
            outputModule.setGainDecibels(trim);
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kCutoff:
        {
            assertRange(parameterValue, 20.0f, 20000.0f);
            cutoff = parameterValue - 1.0;
            bandOneFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, cutoff);
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kBandMode:
        {
            assertRange(parameterValue, 0, 2);
            
            if (parameterValue == 0)
            {
                bandMode = BandMode::kAllBands;
            }
            
            else if (parameterValue == 1)
            {
                bandMode = BandMode::kLowBand;
            }
            
            else
            {
                bandMode = BandMode::kHighBand;
            }
            
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kModelMode:
        {
            if (parameterValue == 0)
            {
                modelMode = ModelMode::kInflator;
            }
            
            else if (parameterValue == 1)
            {
                modelMode = ModelMode::kSaturation;
            }
            
            else if (parameterValue == 2)
            {
                modelMode = ModelMode::kTube;
            }
            
            else if (parameterValue == 3)
            {
                modelMode = ModelMode::kSoft;
            }
            
            else if (parameterValue == 4)
            {
                modelMode = ModelMode::kFold;
            }
            
            else if (parameterValue == 5)
            {
                modelMode = ModelMode::kVari;
            }
            
            else if (parameterValue == 6)
            {
                modelMode = ModelMode::kSine;
            }
            
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kClipInput:
        {
            clipInput = parameterValue;
            std::cout << clipInput << std::endl;
            break;
        }
            
        case LV_SplitDistortion::ParameterId::kBypass:
        {
            assertBool(parameterValue);
            bypassModule = parameterValue;
            break;
        }
    }
}

void LV_SplitDistortion::assertRange(float sourceValue, float min, float max) const
{
    if (sourceValue < min || sourceValue > max) jassertfalse;
}

void LV_SplitDistortion::assertBool(float sourceValue) const
{
    // If you hit this assert, you didn't
    // pass a bool to this parameter call!
    if (sourceValue != 0.0f || sourceValue != 1.0f) jassertfalse;
}

float LV_SplitDistortion::logScale(float value, float scalar) const
{
    return std::pow(10, value / scalar);
}

void LV_SplitDistortion::prepareTopBandFilter(juce::dsp::ProcessSpec& spec)
{
    hpFilter.prepare(spec);
    hpFilter.setCutoffFrequency(cutoff);
    hpFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    
    phaseFilter.prepare(spec);
    phaseFilter.setCutoffFrequency(cutoff);
    phaseFilter.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    
    bandOneFilter.prepare(spec);
    bandOneFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kLowPass);
    bandOneFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, cutoff);
    
    bandTwoFilter.prepare(spec);
    bandTwoFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kHighPass);
    bandTwoFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, cutoff);
}
