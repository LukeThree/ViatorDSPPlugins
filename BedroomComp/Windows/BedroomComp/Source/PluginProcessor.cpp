/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BedroomCompAudioProcessor::BedroomCompAudioProcessor()
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
    treeState.addParameterListener (threshId, this);
    treeState.addParameterListener (ratioId, this);
    treeState.addParameterListener (speedId, this);
    treeState.addParameterListener (blendId, this);
    treeState.addParameterListener (makeUpId, this);
    treeState.addParameterListener (compressorEnableId, this);
    treeState.addParameterListener (hpfId, this);

    //speedDialString = speedValueStrings[*treeState.getRawParameterValue(speedId)];
    compressorModule.setParameter(LV_Compressor::ParameterId::kSpeedValueStringsVec, *treeState.getRawParameterValue(speedId));

}

BedroomCompAudioProcessor::~BedroomCompAudioProcessor()
{
    treeState.removeParameterListener (threshId, this);
    treeState.removeParameterListener (ratioId, this);
    treeState.removeParameterListener (speedId, this);
    treeState.removeParameterListener (blendId, this);
    treeState.removeParameterListener (makeUpId, this);
    treeState.removeParameterListener (compressorEnableId, this);
    treeState.removeParameterListener (hpfId, this);

}

//==============================================================================
const juce::String BedroomCompAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BedroomCompAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BedroomCompAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BedroomCompAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BedroomCompAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BedroomCompAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BedroomCompAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BedroomCompAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BedroomCompAudioProcessor::getProgramName (int index)
{
    return {};
}

void BedroomCompAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BedroomCompAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
        
    compressorModule.setParameter(LV_Compressor::ParameterId::kSampleRate, sampleRate);
    compressorModule.setParameter(LV_Compressor::ParameterId::kRatio, *treeState.getRawParameterValue(ratioId));
    compressorModule.setParameter(LV_Compressor::ParameterId::kSpeedValueStringsVec, *treeState.getRawParameterValue(speedId));
    compressorModule.setParameter(LV_Compressor::ParameterId::kThresh, *treeState.getRawParameterValue(threshId));
    compressorModule.setParameter(LV_Compressor::ParameterId::kTwoDSpeedValues, *treeState.getRawParameterValue(speedId));
    compressorModule.setParameter(LV_Compressor::ParameterId::kBlend, *treeState.getRawParameterValue(blendId));
    compressorModule.setParameter(LV_Compressor::ParameterId::kMakeUp, *treeState.getRawParameterValue(makeUpId));
    compressorModule.setParameter(LV_Compressor::ParameterId::kEnabled, *treeState.getRawParameterValue(compressorEnableId));

    for (auto i {0}; i < hpFilters.size(); i++)
    {
        hpFilters[i]->reset();
        hpFilters[i]->prepare(spec);
        
        hpFilters[i]->setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::FilterType::kHighPass);
        hpFilters[i]->setParameter(LV_SVFilter::ParameterId::kCutoff, *treeState.getRawParameterValue(hpfId));
        hpFilters[i]->setParameter(LV_SVFilter::ParameterId::kQ, 0.5);
        hpFilters[i]->setParameter(LV_SVFilter::ParameterId::kGain, 0.0);
        hpFilters[i]->setParameter(LV_SVFilter::ParameterId::kSampleRate, sampleRate);
    }
    
    //compressorFilterModule.setParameter(LV_SVFilter::ParameterId::kBypass, *treeState.getRawParameterValue(hpfEnabledId));
    //inputHPFilterModule.setParameter(LV_SVFilter::ParameterId::kBypass, !*treeState.getRawParameterValue(hpfEnabledId));
}

void BedroomCompAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BedroomCompAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BedroomCompAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto channelBuffers = buffer.getArrayOfWritePointers();

    for (auto sample {0}; sample < buffer.getNumSamples(); sample++)
    {
        
        for (auto channel {0}; channel < buffer.getNumChannels(); channel++)
        {
            
            auto x = channelBuffers[channel][sample];
            
            /**Turn on input filter if comp filter is off*/
            if (!*treeState.getRawParameterValue(hpfEnabledId))
            {
                channelBuffers[channel][sample] = inputHPFilterModule.processSample(x, channel);
                x = channelBuffers[channel][sample];
            }
            
            else
            {
                channelBuffers[channel][sample] = x;
                x = channelBuffers[channel][sample];
            }
            
            // Do compression
            channelBuffers[channel][sample] = compressorModule.processSample(x, compressorFilterModule.processSample(x, channel), *treeState.getRawParameterValue(hpfEnabledId));
        }
    }
}

float BedroomCompAudioProcessor::getSignal()
{
    return currentSignal;
}

float BedroomCompAudioProcessor::getSampleRate()
{
    return currentSampleRate;
}

float BedroomCompAudioProcessor::getBufferSize()
{
    return currentBufferSize;
}
    


//==============================================================================
bool BedroomCompAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BedroomCompAudioProcessor::createEditor()
{
    return new BedroomCompAudioProcessorEditor (*this);
}

//==============================================================================
void BedroomCompAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
        
    treeState.state.writeToStream (stream);
}

void BedroomCompAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    
    if (tree.isValid())
    {
        treeState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BedroomCompAudioProcessor();
}
