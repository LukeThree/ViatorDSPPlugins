/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionMeetupAudioProcessor::DistortionMeetupAudioProcessor()
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
    treeState.addParameterListener (inputID, this);
    treeState.addParameterListener (driveID, this);
    treeState.addParameterListener (crossOverID, this);
    treeState.addParameterListener (bandChoiceID, this);
    treeState.addParameterListener (modelChoiceID, this);
    treeState.addParameterListener (mixID, this);
    treeState.addParameterListener (outputID, this);
    treeState.addParameterListener (clipInputID, this);
}

DistortionMeetupAudioProcessor::~DistortionMeetupAudioProcessor()
{
    treeState.removeParameterListener (inputID, this);
    treeState.removeParameterListener (driveID, this);
    treeState.removeParameterListener (crossOverID, this);
    treeState.removeParameterListener (bandChoiceID, this);
    treeState.removeParameterListener (modelChoiceID, this);
    treeState.removeParameterListener (mixID, this);
    treeState.removeParameterListener (outputID, this);
    treeState.removeParameterListener (clipInputID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionMeetupAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

  juce::StringArray bands = {"All Bands", "Low Band", "High Band"};
  juce::StringArray models = {"Inflator", "Saturation", "Tube", "Soft", "Fold", "Vari", "Sine"};

  auto pBandChoice = std::make_unique<juce::AudioParameterChoice>(bandChoiceID, bandChoiceName, bands, 0);
  auto pModelChoice = std::make_unique<juce::AudioParameterChoice>(modelChoiceID, modelChoiceName, models, 0);
  auto pClipInput = std::make_unique<juce::AudioParameterBool>(clipInputID, clipInputName, false);
  auto pInput = std::make_unique<juce::AudioParameterFloat>(inputID, inputName, -6.0, 12.0, 0.0);
  auto pDrive = std::make_unique<juce::AudioParameterFloat>(driveID, driveName, -50.0, 50.0, 0.0);
  auto pCrossOver = std::make_unique<juce::AudioParameterFloat>(crossOverID, crossOverName, juce::NormalisableRange<float>(20.0, 20000.0, 1.0, 0.22), 2500.0);
  auto pMix = std::make_unique<juce::AudioParameterInt>(mixID, mixName, 0, 100, 0);
  auto pOutput = std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -24.0, 24.0, 0.0);
  
  params.push_back(std::move(pBandChoice));
  params.push_back(std::move(pModelChoice));
  params.push_back(std::move(pClipInput));
  params.push_back(std::move(pInput));
  params.push_back(std::move(pDrive));
  params.push_back(std::move(pCrossOver));
  params.push_back(std::move(pMix));
  params.push_back(std::move(pOutput));

  return { params.begin(), params.end() };
}

void DistortionMeetupAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == inputID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kPreamp, newValue);
    }
    
    if (parameterID == driveID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCurve, newValue);
    }
    
    if (parameterID == mixID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kWet, newValue);
    }
    
    if (parameterID == outputID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kTrim, newValue);
    }
    
    if (parameterID == crossOverID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCutoff, newValue);
    }
    
    if (parameterID == bandChoiceID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kBandMode, newValue);
    }
    
    if (parameterID == modelChoiceID)
    {
        DBG("Model Mode: " << newValue);
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kModelMode, newValue);
    }
    
    if (parameterID == clipInputID)
    {
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kClipInput, newValue);
    }
}

//==============================================================================
const juce::String DistortionMeetupAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionMeetupAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionMeetupAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionMeetupAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionMeetupAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionMeetupAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionMeetupAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionMeetupAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionMeetupAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionMeetupAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionMeetupAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    splitDistortionModule.prepare(spec);
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kPreamp, *treeState.getRawParameterValue(inputID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCurve, *treeState.getRawParameterValue(driveID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kWet, *treeState.getRawParameterValue(mixID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kTrim, *treeState.getRawParameterValue(outputID));
    splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCutoff, *treeState.getRawParameterValue(crossOverID));
}

void DistortionMeetupAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionMeetupAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DistortionMeetupAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    
    splitDistortionModule.processBlock(audioBlock);
}

//==============================================================================
bool DistortionMeetupAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionMeetupAudioProcessor::createEditor()
{
    //return new DistortionMeetupAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionMeetupAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void DistortionMeetupAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    
    if (tree.isValid())
    {
        treeState.state = tree;
        
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kPreamp, *treeState.getRawParameterValue(inputID));
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCurve, *treeState.getRawParameterValue(driveID));
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kWet, *treeState.getRawParameterValue(mixID));
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kTrim, *treeState.getRawParameterValue(outputID));
        splitDistortionModule.setParameter(LV_SplitDistortion::ParameterId::kCutoff, *treeState.getRawParameterValue(crossOverID));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionMeetupAudioProcessor();
}
