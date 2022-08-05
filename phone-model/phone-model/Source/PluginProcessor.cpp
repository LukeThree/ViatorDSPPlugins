/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhonemodelAudioProcessor::PhonemodelAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout()),
lowFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.0f, 100.0f, 0.91f)),
highFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.0f, 3000.0f, 0.1f)),
midToneFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100.0f, 800.0f, 0.707f, 1.0f)),
lowCompensateFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.0f, 60.0f, 0.1f)),
lowShelfFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100.0f, 60.0f, 0.1f, 1.0f)),
highShelfFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100.0f, 300.0f, 0.1f, 1.0f))
#endif
{
    treeState.addParameterListener (bandwidthID, this);
    treeState.addParameterListener (centerID, this);
    treeState.addParameterListener (driveID, this);
    treeState.addParameterListener (resoID, this);
    treeState.addParameterListener (lowShelfID, this);
    treeState.addParameterListener (highShelfID, this);
}

PhonemodelAudioProcessor::~PhonemodelAudioProcessor()
{
    treeState.removeParameterListener (bandwidthID, this);
    treeState.removeParameterListener (centerID, this);
    treeState.removeParameterListener (driveID, this);
    treeState.removeParameterListener (resoID, this);
    treeState.removeParameterListener (lowShelfID, this);
    treeState.removeParameterListener (highShelfID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout PhonemodelAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

  // Make sure to update the number of reservations after adding params
  params.reserve(6);

  auto pBandWidth = std::make_unique<juce::AudioParameterFloat>(bandwidthID, bandwidthName, 0.0, 100.0, 70.0);
  auto pCenter = std::make_unique<juce::AudioParameterFloat>(centerID, centerName, 100.0, 3000.0, 800.0);
  auto pDrive = std::make_unique<juce::AudioParameterFloat>(driveID, driveName, 0.0, 12.0, 4.0);
  auto pReso = std::make_unique<juce::AudioParameterInt>(resoID, resoName, 0, 100, 70);
  auto pLowShelf = std::make_unique<juce::AudioParameterFloat>(lowShelfID, lowShelfName, 0.0, 12.0, 0.0);
  auto pHighShelf = std::make_unique<juce::AudioParameterFloat>(highShelfID, highShelfName, 0.0, 12.0, 0.0);
    
  params.push_back(std::move(pBandWidth));
  params.push_back(std::move(pCenter));
  params.push_back(std::move(pDrive));
  params.push_back(std::move(pReso));
  params.push_back(std::move(pLowShelf));
  params.push_back(std::move(pHighShelf));
    
  return { params.begin(), params.end() };
}

void PhonemodelAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == bandwidthID)
    {
        lowCutoffMultiplier = juce::jmap(newValue, 0.0f, 100.0f, 0.5f, 0.125f);
        highCutoffMultiplier = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 4.0f);
    }
    
    else if (parameterID == centerID)
    {
        lowCutoff = calcLowCutoff(newValue);
        highCutoff = calcHighCutoff(newValue);
        centerFreq = newValue;
    }
    
    else if (parameterID == driveID)
    {
        drive = juce::jmap(newValue, 0.0f, 12.0f, 1.0f, 0.45f);
        inputRectifierModule.setParameter(LV_Rectifier::ParameterId::kDrive, drive);
        outputRectifierModule.setParameter(LV_Rectifier::ParameterId::kDrive, drive);
    }
    
    else if (parameterID == lowShelfID)
    {
        lowShelfGain = juce::Decibels::decibelsToGain(newValue * 2.0f);
        updateLowShelfFilter();
    }
    
    else if (parameterID == highShelfID)
    {
        highShelfGain = juce::Decibels::decibelsToGain(newValue);
        updateHighShelfFilter();
    }
    
    else
    {
        qWidth = juce::jmap(newValue, 0.0f, 100.0f, 0.5f, 9.0f);
        midGain = juce::jmap(newValue, 0.0f, 100.0f, 0.0f, 12.0f);
    }
    
    updateAllFilters();
}

//==============================================================================
const juce::String PhonemodelAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PhonemodelAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhonemodelAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PhonemodelAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PhonemodelAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PhonemodelAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PhonemodelAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PhonemodelAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PhonemodelAudioProcessor::getProgramName (int index)
{
    return {};
}

void PhonemodelAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PhonemodelAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Process spec
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    // Initialize samplerate
    lastSampleRate = spec.sampleRate;
    
    // Prepare filters
    prepareDSP(spec);
    
    // Init Vars
    initDSPVariables();
    
    // Initial filter updates on new instance
    initFilters();
}

void PhonemodelAudioProcessor::initCutoffMultipliers(float newValue)
{
    lowCutoffMultiplier = juce::jmap(newValue, 0.0f, 100.0f, 0.5f, 0.125f);
    highCutoffMultiplier = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 4.0f);
}

void PhonemodelAudioProcessor::initCutoffs(float newValue)
{
    lowCutoff = calcLowCutoff(newValue);
    highCutoff = calcHighCutoff(newValue);
}

void PhonemodelAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PhonemodelAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PhonemodelAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> block (buffer);

    // Initial rectifier dsp
    inputRectifierModule.processBlock(block);

    // Do filtering
    lowFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    highFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    midToneFilter.process(juce::dsp::ProcessContextReplacing<float>(block));

    // Shape the filtering
    outputRectifierModule.processBlock(block);

    // Filter additional low end from shaper
    lowCompensateFilter.process(juce::dsp::ProcessContextReplacing<float>(block));

    // low shelf
    lowShelfFilter.process(juce::dsp::ProcessContextReplacing<float>(block));

    // high shelf
    highShelfFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void PhonemodelAudioProcessor::updateLowFilter()
{
    *lowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, lowCutoff * lowCutoffMultiplier, 0.91f);
}

void PhonemodelAudioProcessor::updateHighFilter()
{
    *highFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, highCutoff * highCutoffMultiplier);
}

void PhonemodelAudioProcessor::updateMidToneFilter(float freq, float q, float gain)
{
    auto midDrive = juce::Decibels::decibelsToGain(gain);
    *midToneFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, freq, q, midDrive);
}

void PhonemodelAudioProcessor::updateLowCompensateFilter()
{
    *lowCompensateFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, lowCutoff * lowCutoffMultiplier);
}

void PhonemodelAudioProcessor::updateLowShelfFilter()
{
    *lowShelfFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, lowCutoff * lowCutoffMultiplier * 0.125f, 0.1f, lowShelfGain);
}

void PhonemodelAudioProcessor::updateHighShelfFilter()
{
    *highShelfFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, highCutoff * highCutoffMultiplier, 0.5f, highShelfGain);
}

void PhonemodelAudioProcessor::updateAllFilters()
{
    updateLowFilter();
    updateHighFilter();
    updateMidToneFilter(centerFreq, qWidth, midGain);
    updateLowCompensateFilter();
    updateLowShelfFilter();
    updateHighShelfFilter();
}

float PhonemodelAudioProcessor::calcLowCutoff(float centerFreq)
{
    return centerFreq * 0.9f;
}

float PhonemodelAudioProcessor::calcHighCutoff(float centerFreq)
{
    return centerFreq * 0.75f + centerFreq;
}

int PhonemodelAudioProcessor::initDN() const
{
    return 69;
}

//==============================================================================
bool PhonemodelAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PhonemodelAudioProcessor::createEditor()
{
    return new PhonemodelAudioProcessorEditor (*this);
}

//==============================================================================
void PhonemodelAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void PhonemodelAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        treeState.state = tree;
    }
    
    windowWidth = variableTree.getProperty("width");
    windowHeight = variableTree.getProperty("height");
    
    // Init Vars
    initDSPVariables();
    
    // Initial filter updates on new instance
    initFilters();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhonemodelAudioProcessor();
}
