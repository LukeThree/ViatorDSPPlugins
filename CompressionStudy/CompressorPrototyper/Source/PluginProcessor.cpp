/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorPrototyperAudioProcessor::CompressorPrototyperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

CompressorPrototyperAudioProcessor::~CompressorPrototyperAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorPrototyperAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(6);
    
    
    auto inputGainParam = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -36.0f, 36.0f, 0.0f);
    auto ratioParam = std::make_unique<juce::AudioParameterInt>(ratioSliderId, ratioSliderName, 1.0f, 10.0f, 1.0f);
    auto threshParam = std::make_unique<juce::AudioParameterFloat>(threshSliderId, threshSliderName, -30.0f, 0.0f, 0.0f);
    auto attackParam = std::make_unique<juce::AudioParameterInt>(attackSliderId, attackSliderName, 1.0f, 1000.0f, 500.0f);
    auto releaseParam = std::make_unique<juce::AudioParameterFloat>(releaseSliderId, releaseSliderName, 10.0f, 430.0f, 100.0f);
    auto outputGainParam = std::make_unique<juce::AudioParameterFloat>(outputGainSliderId, outputGainSliderName, -36.0f, 36.0f, 0.0f);
    
    params.push_back(std::move(inputGainParam));
    params.push_back(std::move(ratioParam));
    params.push_back(std::move(threshParam));
    params.push_back(std::move(attackParam));
    params.push_back(std::move(releaseParam));
    params.push_back(std::move(outputGainParam));
    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String CompressorPrototyperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorPrototyperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorPrototyperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorPrototyperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorPrototyperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorPrototyperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorPrototyperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorPrototyperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompressorPrototyperAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorPrototyperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorPrototyperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    inputGainProcessor.prepare(spec);
    compressorProcessor.prepare(spec);
    outputGainProcessor.prepare(spec);
}

void CompressorPrototyperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorPrototyperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void CompressorPrototyperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock {buffer};

    auto* rawInput = treeState.getRawParameterValue(inputGainSliderId);
    auto* rawRatio = treeState.getRawParameterValue(ratioSliderId);
    auto* rawThresh = treeState.getRawParameterValue(threshSliderId);
    auto* rawAttack = treeState.getRawParameterValue(attackSliderId);
    auto* rawRelease = treeState.getRawParameterValue(releaseSliderId);
    auto* rawTrim = treeState.getRawParameterValue(outputGainSliderId);
    
    inputGainProcessor.setGainDecibels(*rawInput);
    inputGainProcessor.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));

    compressorProcessor.setRatio(*rawRatio);
    compressorProcessor.setThreshold(*rawThresh - 30);
    compressorProcessor.setAttack(*rawAttack);
    compressorProcessor.setRelease(*rawRelease);
    compressorProcessor.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));

    outputGainProcessor.setGainDecibels(*rawTrim);
    outputGainProcessor.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
}

//==============================================================================
bool CompressorPrototyperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorPrototyperAudioProcessor::createEditor()
{
    return new CompressorPrototyperAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorPrototyperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
        treeState.state.writeToStream (stream);
}

void CompressorPrototyperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new CompressorPrototyperAudioProcessor();
}
