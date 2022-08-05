/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Distortion_StudyAudioProcessor::Distortion_StudyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

Distortion_StudyAudioProcessor::~Distortion_StudyAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Distortion_StudyAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(3);
    
    
    auto inputGainParam = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -48, 48, 0);
    auto biasVoltageParam = std::make_unique<juce::AudioParameterFloat>(biasSliderId, biasSliderName, -1.0f, 1.0f, 0);
    auto outputGainParam = std::make_unique<juce::AudioParameterFloat>(outputGainSliderId, outputGainSliderName, -48, 48, 0);
    
    params.push_back(std::move(inputGainParam));
    params.push_back(std::move(biasVoltageParam));
    params.push_back(std::move(outputGainParam));
    

    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String Distortion_StudyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Distortion_StudyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Distortion_StudyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Distortion_StudyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Distortion_StudyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Distortion_StudyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Distortion_StudyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Distortion_StudyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Distortion_StudyAudioProcessor::getProgramName (int index)
{
    return {};
}

void Distortion_StudyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Distortion_StudyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    distortionProcessor.prepare(spec);
    distortionProcessor.get<1>().setRampDurationSeconds(1.0f);
    distortionProcessor.get<2>().functionToUse = juce::dsp::FastMathApproximations::tanh;
}

void Distortion_StudyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Distortion_StudyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Distortion_StudyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    juce::dsp::AudioBlock<float> audioBlock (buffer);
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto* rawInputGain = treeState.getRawParameterValue(inputGainSliderId);
    distortionProcessor.get<0>().setGainDecibels(*rawInputGain);
    
    auto* rawBiasVoltage = treeState.getRawParameterValue(biasSliderId);
    distortionProcessor.get<1>().setBias(*rawBiasVoltage);
        
    auto* rawOutputGain = treeState.getRawParameterValue(outputGainSliderId);
    distortionProcessor.get<3>().setGainDecibels(*rawOutputGain);
    
    distortionProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
}

//==============================================================================
bool Distortion_StudyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Distortion_StudyAudioProcessor::createEditor()
{
    return new Distortion_StudyAudioProcessorEditor (*this);
}

//==============================================================================
void Distortion_StudyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void Distortion_StudyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
    if (tree.isValid()) {
        treeState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Distortion_StudyAudioProcessor();
}
