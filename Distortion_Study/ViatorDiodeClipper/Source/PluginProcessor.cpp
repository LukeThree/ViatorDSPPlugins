/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViatorDiodeClipperAudioProcessor::ViatorDiodeClipperAudioProcessor()
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

ViatorDiodeClipperAudioProcessor::~ViatorDiodeClipperAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ViatorDiodeClipperAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(3);
    
    
    auto inputParam = std::make_unique<juce::AudioParameterFloat>(inputSliderId, inputSliderName, -24, 24, 0);
    auto driveParam = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0, 24, 0);
    auto trimParam = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -24, 24, 0);
    
    params.push_back(std::move(inputParam));
    params.push_back(std::move(driveParam));
    params.push_back(std::move(trimParam));
    

    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String ViatorDiodeClipperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ViatorDiodeClipperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ViatorDiodeClipperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ViatorDiodeClipperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ViatorDiodeClipperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ViatorDiodeClipperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ViatorDiodeClipperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ViatorDiodeClipperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ViatorDiodeClipperAudioProcessor::getProgramName (int index)
{
    return {};
}

void ViatorDiodeClipperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ViatorDiodeClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    inputGainProcessor.prepare(spec);
    outputGainProcessor.prepare(spec);
}

void ViatorDiodeClipperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ViatorDiodeClipperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ViatorDiodeClipperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto* rawInput = treeState.getRawParameterValue(inputSliderId);
    auto* rawDrive = treeState.getRawParameterValue(driveSliderId);
    auto* rawTrim = treeState.getRawParameterValue(trimSliderId);
    
    juce::dsp::AudioBlock<float> audioBlock (buffer);
    
//    inputGainProcessor.setGainDecibels(*rawInput);
//    inputGainProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* outputData = buffer.getWritePointer (channel);
        auto* inputData = buffer.getReadPointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            outputData[sample] = (exp((((pow(10, (*rawInput * .05)) * .1) * inputData[sample]) / (scaleRange(*rawDrive, 0.0f, 24.0f, 2.0f, 1.0f) * scaleRange(*rawDrive, 0.0f, 24.0, 0.04f, 0.01)))) - 1);
        }
    }
    
    outputGainProcessor.setGainDecibels(*rawTrim - ((*rawInput + *rawDrive)));
    outputGainProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

float ViatorDiodeClipperAudioProcessor::scaleRange(float input, float inputLow, float inputHigh, float outputLow, float outputHigh){
    return ((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow;

}

//==============================================================================
bool ViatorDiodeClipperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ViatorDiodeClipperAudioProcessor::createEditor()
{
    return new ViatorDiodeClipperAudioProcessorEditor (*this);
}

//==============================================================================
void ViatorDiodeClipperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ViatorDiodeClipperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ViatorDiodeClipperAudioProcessor();
}
