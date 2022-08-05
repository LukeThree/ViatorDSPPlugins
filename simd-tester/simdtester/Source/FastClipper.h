/*
  ==============================================================================

    FastClipper.h
    Created: 2 Mar 2022 1:51:22pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FastClipperModule.h"

class FastClipper
{
public:
    
    void prepare(juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        interleaved = juce::dsp::AudioBlock<juce::dsp::SIMDRegister<float>> (interleavedBlockData, 1, spec.maximumBlockSize);
        zero        = juce::dsp::AudioBlock<float> (zeroData, juce::dsp::SIMDRegister<float>::size(), spec.maximumBlockSize);

        zero.clear();

        auto monoSpec = spec;
        monoSpec.numChannels = 1;
        
        clipper1->prepare(spec);
        clipper2->prepare(spec);
        clipper3->prepare(spec);
        clipper4->prepare(spec);
    }
    
    void process (const juce::dsp::ProcessContextReplacing<float>& context)
    {
        jassert (context.getInputBlock().getNumSamples()  == context.getOutputBlock().getNumSamples());
        jassert (context.getInputBlock().getNumChannels() == context.getOutputBlock().getNumChannels());

        auto& input  = context.getInputBlock();
        auto& output = context.getOutputBlock();
        auto n = input.getNumSamples();
        auto* inout = channelPointers.getData();


        for (size_t ch = 0; ch < juce::dsp::SIMDRegister<float>::size(); ++ch)
            inout[ch] = (ch < input.getNumChannels() ? const_cast<float*> (input.getChannelPointer (ch)) : zero.getChannelPointer (ch));

        juce::AudioDataConverters::interleaveSamples (inout, reinterpret_cast<float*> (interleaved.getChannelPointer (0)),
                                                static_cast<int> (n), static_cast<int> (juce::dsp::SIMDRegister<float>::size()));


        clipper1->process (juce::dsp::ProcessContextReplacing<juce::dsp::SIMDRegister<float>> (interleaved));
        clipper2->process (juce::dsp::ProcessContextReplacing<juce::dsp::SIMDRegister<float>> (interleaved));
        clipper3->process (juce::dsp::ProcessContextReplacing<juce::dsp::SIMDRegister<float>> (interleaved));
        clipper4->process (juce::dsp::ProcessContextReplacing<juce::dsp::SIMDRegister<float>> (interleaved));


        for (size_t ch = 0; ch < input.getNumChannels(); ++ch)
            inout[ch] = output.getChannelPointer (ch);

        juce::AudioDataConverters::deinterleaveSamples (reinterpret_cast<float*> (interleaved.getChannelPointer (0)),
                                                  const_cast<float**> (inout),
                                                  static_cast<int> (n), static_cast<int> (juce::dsp::SIMDRegister<float>::size()));
    }
    
    void setPreamp(float newPreamp)
    {
        clipper1->setParameter(viator_dsp::FastClipperModule::ParameterId::kPreamp, newPreamp);
        clipper2->setParameter(viator_dsp::FastClipperModule::ParameterId::kPreamp, newPreamp);
        clipper3->setParameter(viator_dsp::FastClipperModule::ParameterId::kPreamp, newPreamp);
        clipper4->setParameter(viator_dsp::FastClipperModule::ParameterId::kPreamp, newPreamp);
    }
    
private:
    std::unique_ptr<viator_dsp::FastClipperModule> clipper1;
    std::unique_ptr<viator_dsp::FastClipperModule> clipper2;
    std::unique_ptr<viator_dsp::FastClipperModule> clipper3;
    std::unique_ptr<viator_dsp::FastClipperModule> clipper4;
    
    juce::dsp::AudioBlock<juce::dsp::SIMDRegister<float>> interleaved;
    juce::dsp::AudioBlock<float> zero;

    juce::HeapBlock<char> interleavedBlockData, zeroData;
    juce::HeapBlock<const float*> channelPointers { juce::dsp::SIMDRegister<float>::size() };
    
    double sampleRate = 0.0;
};
