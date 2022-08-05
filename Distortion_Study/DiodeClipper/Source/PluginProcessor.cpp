/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DiodeClipperAudioProcessor::DiodeClipperAudioProcessor()
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

DiodeClipperAudioProcessor::~DiodeClipperAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DiodeClipperAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(3);
    
    
    auto thermalVoltageParam = std::make_unique<juce::AudioParameterFloat>(thermalVoltageSliderId, thermalVoltageSliderName, 0.001f, 0.09f, 0.0253f);
    auto emissionCoefficientParam = std::make_unique<juce::AudioParameterFloat>(emissionCoefficientSliderId, emissionCoefficientSliderName, 1.0f, 2.0f, 1.68f);
    auto saturationCurrentParam = std::make_unique<juce::AudioParameterFloat>(saturationCurrentSliderId, saturationCurrentSliderName, 0.001f, 1.0f, 0.105f);
    
    params.push_back(std::move(thermalVoltageParam));
    params.push_back(std::move(emissionCoefficientParam));
    params.push_back(std::move(saturationCurrentParam));
    

    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String DiodeClipperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DiodeClipperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DiodeClipperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DiodeClipperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DiodeClipperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DiodeClipperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DiodeClipperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DiodeClipperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DiodeClipperAudioProcessor::getProgramName (int index)
{
    return {};
}

void DiodeClipperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DiodeClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    outputGainProcessor.prepare(spec);
}

void DiodeClipperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DiodeClipperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DiodeClipperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* outputData = buffer.getWritePointer (channel);
        auto* inputData = buffer.getReadPointer(channel);
        auto* rawThermalVoltage = treeState.getRawParameterValue(thermalVoltageSliderId);
        auto* rawEmissionCoefficient = treeState.getRawParameterValue(emissionCoefficientSliderId);
        auto* rawSaturationCurrent = treeState.getRawParameterValue(saturationCurrentSliderId);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            outputData[sample] = exp(inputData[sample] / (0.0253 * 1.68)) - 1;
        }
    }
}

//Saturation Current = 1;
//Drive -> Emission Coefficient [2...1]
//Drive -> Thermal Voltage [.04....01]
//Expecting -15 to -18 dB of signal
//Need an input knob to multiply the input where is says "0.1f" now
//Drive will scale the Emission Current and Thermal Voltage
//Will need a trim knob as an output multiplier with automatic compensation as the drive and input goes up
//This will all be one setting in the channel strip plugin. The user will be able to select different console saturation circuits, this one being the diode circuit

//==============================================================================
bool DiodeClipperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DiodeClipperAudioProcessor::createEditor()
{
    return new DiodeClipperAudioProcessorEditor (*this);
}

//==============================================================================
void DiodeClipperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DiodeClipperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DiodeClipperAudioProcessor();
}
