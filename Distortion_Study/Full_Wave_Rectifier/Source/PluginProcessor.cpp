/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Full_Wave_RectifierAudioProcessor::Full_Wave_RectifierAudioProcessor()
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

Full_Wave_RectifierAudioProcessor::~Full_Wave_RectifierAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Full_Wave_RectifierAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(2);
    
    auto inputParam = std::make_unique<juce::AudioParameterFloat>(inputSliderId, inputSliderName, -24.0f, 24.0f, 0.0f);
    auto trimParam = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -24.0f, 24.0f, 0.0f);

    params.push_back(std::move(inputParam));
    params.push_back(std::move(trimParam));
    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String Full_Wave_RectifierAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Full_Wave_RectifierAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Full_Wave_RectifierAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Full_Wave_RectifierAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Full_Wave_RectifierAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Full_Wave_RectifierAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Full_Wave_RectifierAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Full_Wave_RectifierAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Full_Wave_RectifierAudioProcessor::getProgramName (int index)
{
    return {};
}

void Full_Wave_RectifierAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Full_Wave_RectifierAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Full_Wave_RectifierAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Full_Wave_RectifierAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Full_Wave_RectifierAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* inputData = buffer.getReadPointer (channel);
        auto* outputData = buffer.getWritePointer (channel);
                
        auto* rawInput = treeState.getRawParameterValue(inputSliderId);
        auto* rawTrim = treeState.getRawParameterValue(trimSliderId);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
                    
            auto input = inputData[sample] * pow(10, *rawInput * 0.05);
                    
            if (input >= 0){
                outputData[sample] = input * pow(10, *rawTrim * 0.05);
            } else {
                outputData[sample] = (input * -1) * pow(10, *rawTrim * 0.05);
            }
        }
    }
}

//==============================================================================
bool Full_Wave_RectifierAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Full_Wave_RectifierAudioProcessor::createEditor()
{
    return new Full_Wave_RectifierAudioProcessorEditor (*this);
}

//==============================================================================
void Full_Wave_RectifierAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void Full_Wave_RectifierAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new Full_Wave_RectifierAudioProcessor();
}
