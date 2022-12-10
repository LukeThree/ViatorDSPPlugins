/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SweetenerAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    SweetenerAudioProcessor();
    ~SweetenerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState treeState;
    
    juce::ValueTree variableTree
    { "Variables", {},
        {
        { "Group", {{ "name", "Vars" }},
            {
                { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                { "Parameter", {{ "id", "height" }, { "value", 0.0 }}}
            }
        }
        }
    };
    
    /** Window Vars =====================================================*/
    float windowWidth {0.0f};
    float windowHeight {0.0f};
    
    using Range = juce::NormalisableRange<float>;

    /* min, max */
    Range foleysLogRange(float min, float max) noexcept
    {
        return
        {
            min, max,
            [](float start, float end, float normalised)
            {
                return start + (std::pow(2.f, normalised * 10.f) - 1.f) * (end - start) / 1023.f;
            },
            [](float start, float end, float value)
            {
                return (std::log(((value - start) * 1023.f / (end - start)) + 1.f) / std::log(2.f)) * .1f;
            },
            [](float start, float end, float value)
            {
                // optimised for frequencies: >3 kHz: 2 decimals
                if (value > 3000.f)
                    return juce::jlimit(start, end, 100.f * juce::roundToInt(value / 100.f));

                // optimised for frequencies: 1-3 kHz: 1 decimal
                if (value > 1000.f)
                    return juce::jlimit(start, end, 10.f * juce::roundToInt(value * .1f));

                return juce::jlimit(start, end, std::round(value));
            }
        };
    }
    
    Range biased(float start, float end, float bias) noexcept
    {
        // https://www.desmos.com/calculator/ps8q8gftcr
        const auto a = bias * .5f + .5f;
        const auto a2 = 2.f * a;
        const auto aM = 1.f - a;

        const auto r = end - start;
        const auto aR = r * a;
        if (bias != 0.f)
            return
        {
                start, end,
                [a2, aM, aR](float min, float, float x)
                {
                    const auto denom = aM - x + a2 * x;
                    if (denom == 0.f)
                        return min;
                    return min + aR * x / denom;
                },
                [a2, aM, aR](float min, float, float x)
                {
                    const auto denom = a2 * min + aR - a2 * x - min + x;
                    if (denom == 0.f)
                        return 0.f;
                    auto val = aM * (x - min) / denom;
                    return val > 1.f ? 1.f : val;
                },
                [](float min, float max, float x)
                {
                    return x < min ? min : x > max ? max : x;
                }
        };
        else return { start, end };
    }
    
    Range withCentre(float start, float end, float centre) noexcept
    {
        const auto r = end - start;
        const auto v = (centre - start) / r;

        return biased(start, end, 2.f * v - 1.f);
    }

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SweetenerAudioProcessor)
};
