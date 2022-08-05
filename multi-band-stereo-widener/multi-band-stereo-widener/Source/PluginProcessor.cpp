/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultibandstereowidenerAudioProcessor::MultibandstereowidenerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), false)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

MultibandstereowidenerAudioProcessor::~MultibandstereowidenerAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MultibandstereowidenerAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

  // Make sure to update the number of reservations after adding params
  params.reserve(3);
    
  auto pLowWidth = std::make_unique<juce::AudioParameterFloat>(lowWidthId, lowWidthName, 0.0, 2.0, 1.0);
  auto pHighWidth = std::make_unique<juce::AudioParameterFloat>(highWidthId, highWidthName, 0.0, 2.0, 1.0);
  auto pCrossover = std::make_unique<juce::AudioParameterInt>(crossoverId, crossoverName, 60, 16000, 1000);

  params.push_back(std::move(pLowWidth));
  params.push_back(std::move(pHighWidth));
  params.push_back(std::move(pCrossover));
    
  return { params.begin(), params.end() };
}

void MultibandstereowidenerAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{

}

//==============================================================================
const juce::String MultibandstereowidenerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultibandstereowidenerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultibandstereowidenerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultibandstereowidenerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultibandstereowidenerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultibandstereowidenerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultibandstereowidenerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultibandstereowidenerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultibandstereowidenerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultibandstereowidenerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultibandstereowidenerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    float lowWidth = static_cast<float>(*treeState.getRawParameterValue(lowWidthId));
    float highWidth = static_cast<float>(*treeState.getRawParameterValue(highWidthId));
    float crossover = static_cast<float>(*treeState.getRawParameterValue(crossoverId));
    
    widenerModule.prepare(spec);
    widenerModule.setParameter(LV_Widener::ParameterId::kLowWidth, lowWidth);
    widenerModule.setParameter(LV_Widener::ParameterId::kHighWidth, highWidth);
    widenerModule.setParameter(LV_Widener::ParameterId::kCrossover, crossover);
}

void MultibandstereowidenerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultibandstereowidenerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MultibandstereowidenerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

        juce::dsp::AudioBlock<float> audioBlock {buffer};
        widenerModule.setParameter(LV_Widener::ParameterId::kLowWidth, lowWidth);
        widenerModule.setParameter(LV_Widener::ParameterId::kHighWidth, highWidth);
        widenerModule.setParameter(LV_Widener::ParameterId::kCrossover, crossover);
        widenerModule.processBlock(audioBlock);
}

//==============================================================================
bool MultibandstereowidenerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultibandstereowidenerAudioProcessor::createEditor()
{
    return new MultibandstereowidenerAudioProcessorEditor (*this);
}

//==============================================================================
void MultibandstereowidenerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void MultibandstereowidenerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        treeState.state = tree;
        windowWidth = variableTree.getProperty("width");
        windowHeight = variableTree.getProperty("height");
        
        float lowWidth = static_cast<float>(*treeState.getRawParameterValue(lowWidthId));
        float highWidth = static_cast<float>(*treeState.getRawParameterValue(highWidthId));
        float crossover = static_cast<float>(*treeState.getRawParameterValue(crossoverId));
        
        widenerModule.setParameter(LV_Widener::ParameterId::kLowWidth, lowWidth);
        widenerModule.setParameter(LV_Widener::ParameterId::kHighWidth, highWidth);
        widenerModule.setParameter(LV_Widener::ParameterId::kCrossover, crossover);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultibandstereowidenerAudioProcessor();
}
