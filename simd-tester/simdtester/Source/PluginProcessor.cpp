/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimdtesterAudioProcessor::SimdtesterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState(*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

SimdtesterAudioProcessor::~SimdtesterAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SimdtesterAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

  // Make sure to update the number of reservations after adding params
  params.reserve(1);
    
  auto pODInput = std::make_unique<juce::AudioParameterFloat>("od input", "OD Input", 0.0, 10.0, 0.0);
    
  params.push_back(std::move(pODInput));
  return { params.begin(), params.end() };
}

//==============================================================================
const juce::String SimdtesterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimdtesterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimdtesterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimdtesterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimdtesterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimdtesterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimdtesterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimdtesterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimdtesterAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimdtesterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimdtesterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    fastClipper.prepare(spec);
}

void SimdtesterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimdtesterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimdtesterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    input = *treeState.getRawParameterValue("od input");
    fastClipper.setPreamp(input);
    
    juce::dsp::AudioBlock<float> block (buffer);
    
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
    
    fastClipper.process(context);
}

//juce::dsp::SIMDRegister<float> SimdtesterAudioProcessor::softClipData(juce::dsp::SIMDRegister<float> dataToClip)
//{
//    /** Don't do anything if the module is off*/
//    //if (mGlobalBypass) return dataToClip;
//    
//    /** Soft Clipping algorithim*/
//    auto softClipper = 3/14 * std::atan(dataToClip) * 2.0;
//    
//    /** Auto gain stage the soft clipper*/
//    softClipper *= viator_utils::utils::dbToGain(-6.0 * 0.25);
//    
//    return softClipper;
//}

//==============================================================================
bool SimdtesterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimdtesterAudioProcessor::createEditor()
{
    //return new SimdtesterAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void SimdtesterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimdtesterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimdtesterAudioProcessor();
}
