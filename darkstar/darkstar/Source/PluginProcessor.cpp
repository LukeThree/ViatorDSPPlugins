/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DarkstarAudioProcessor::DarkstarAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), false)
                     #endif
                       ),
treeState(*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
    treeState.addParameterListener ("od input", this);
    treeState.addParameterListener ("od tone", this);
    treeState.addParameterListener ("od level", this);
}

DarkstarAudioProcessor::~DarkstarAudioProcessor()
{
    treeState.removeParameterListener ("od input", this);
    treeState.removeParameterListener ("od tone", this);
    treeState.removeParameterListener ("od level", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout DarkstarAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

  // Make sure to update the number of reservations after adding params
  params.reserve(3);
    
  auto pODInput = std::make_unique<juce::AudioParameterFloat>("od input", "OD Input", 0.0, 10.0, 0.0);
  auto pODTone = std::make_unique<juce::AudioParameterFloat>("od tone", "OD Tone", -10.0, 10.0, 0.0);
  auto pODShape = std::make_unique<juce::AudioParameterFloat>("od level", "OD Level", -10.0, 10.0, 0.0);
    
  params.push_back(std::move(pODInput));
  params.push_back(std::move(pODTone));
  params.push_back(std::move(pODShape));
  return { params.begin(), params.end() };
}

void DarkstarAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "od input")
    {
        pedalShaper.setParameter(viator_dsp::WaveShaper::ParameterId::kPreamp, newValue);
    }
    
    if (parameterID == "od tone")
    {
        /** Map tone knob to go from 500 Hz to 2000 Hz*/
        auto tone = juce::jmap(newValue, -10.0f, 10.0f, 0.5f, 1.5f);
        pedalLPFilter.setParameter(viator_dsp::SVFilter::ParameterId::kCutoff, 1000.0 * tone);
    }
    
    if (parameterID == "od level")
    {
        pedalLevelModule.setGainDecibels(newValue);
    }
}

//==============================================================================
const juce::String DarkstarAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DarkstarAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DarkstarAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DarkstarAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DarkstarAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DarkstarAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DarkstarAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DarkstarAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DarkstarAudioProcessor::getProgramName (int index)
{
    return {};
}

void DarkstarAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DarkstarAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    pedalLPFilter.prepare(spec);
    pedalLPFilter.setParameter(viator_dsp::SVFilter::ParameterId::kType, viator_dsp::SVFilter::FilterType::kLowPass);
    pedalLPFilter.setParameter(viator_dsp::SVFilter::ParameterId::kCutoff, 1000.0);
    
    pedalHPFilter.prepare(spec);
    pedalHPFilter.setParameter(viator_dsp::SVFilter::ParameterId::kType, viator_dsp::SVFilter::FilterType::kHighPass);
    pedalHPFilter.setParameter(viator_dsp::SVFilter::ParameterId::kCutoff, 300.0);
    
    pedalShaper.prepare(spec);
    pedalShaper.setParameter(viator_dsp::WaveShaper::ParameterId::kPreamp, 0.0);
    
    pedalLevelModule.prepare(spec);
    pedalLevelModule.setGainDecibels(0.0);
}

void DarkstarAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DarkstarAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DarkstarAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> block (buffer);
    pedalShaper.process(juce::dsp::ProcessContextReplacing<float>(block));
    processOverdrivePedal(block);

}

void DarkstarAudioProcessor::processOverdrivePedal(juce::dsp::AudioBlock<float>& inputBlock)
{
    pedalShaper.process(juce::dsp::ProcessContextReplacing<float>(inputBlock));
    pedalHPFilter.processBlock(inputBlock);
    pedalHPFilter.processBlock(inputBlock);
    viator_utils::utils::hardClipBlock(inputBlock);
}

//==============================================================================
bool DarkstarAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DarkstarAudioProcessor::createEditor()
{
    //return new DarkstarAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void DarkstarAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DarkstarAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DarkstarAudioProcessor();
}
