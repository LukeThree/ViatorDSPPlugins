/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Hard_ClipperAudioProcessor::Hard_ClipperAudioProcessor()
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

Hard_ClipperAudioProcessor::~Hard_ClipperAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Hard_ClipperAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(2);
    
    auto inputParam = std::make_unique<juce::AudioParameterFloat>(inputSliderId, inputSliderName, 0.0f, 24.0f, 0.0f);
    auto trimParam = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -24.0f, 24.0f, 0.0f);

    params.push_back(std::move(inputParam));
    params.push_back(std::move(trimParam));
    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String Hard_ClipperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Hard_ClipperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Hard_ClipperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Hard_ClipperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Hard_ClipperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Hard_ClipperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Hard_ClipperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Hard_ClipperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Hard_ClipperAudioProcessor::getProgramName (int index)
{
    return {};
}

void Hard_ClipperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Hard_ClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Hard_ClipperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Hard_ClipperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Hard_ClipperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
                
        auto* rawThresh = treeState.getRawParameterValue(inputSliderId);
        auto* rawTrim = treeState.getRawParameterValue(trimSliderId);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
                    
            auto input = inputData[sample];
            
            //set thresh to be rounded to nearest 100th
            float scale = 0.01;
            
            //make the range short in the first half and long at the end
            auto threshScaled = floor(log10(*rawThresh + 1) / scale + 0.5f) * scale;
            
            //use scale function to reverse the direction of the dial
            auto threshReversed = scaleRange(threshScaled, 0.0f, 1.4f, 1.0f, 0.02f);

            //hard clipping
            if (input >= threshReversed){
                outputData[sample] = threshReversed * pow(10, *rawTrim * 0.05);
            } else if (input <= (threshReversed * -1)) {
                outputData[sample] = (threshReversed * -1) * pow(10, *rawTrim * 0.05);
            } else {
                outputData[sample] = input * pow(10, *rawTrim * 0.05);
            }
        }
    }
}

float Hard_ClipperAudioProcessor::scaleRange(const float &input, const float &inputLow, const float &inputHigh, const float &outputLow, const float &outputHigh){
    return ((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow;
}

//==============================================================================
bool Hard_ClipperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Hard_ClipperAudioProcessor::createEditor()
{
    return new Hard_ClipperAudioProcessorEditor (*this);
}

//==============================================================================
void Hard_ClipperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void Hard_ClipperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new Hard_ClipperAudioProcessor();
}
