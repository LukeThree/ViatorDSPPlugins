/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JustaFilterAudioProcessor::JustaFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), false)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout()),
oversamplingModule(2, 3, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR)
#endif
{
    treeState.addParameterListener (filterTypeID, this);
    treeState.addParameterListener (bandwidthTypeID, this);
    treeState.addParameterListener (cutoffID, this);
    treeState.addParameterListener (bandwidthID, this);
    treeState.addParameterListener (gainID, this);
    treeState.addParameterListener (qualityID, this);
    treeState.addParameterListener (phaseID, this);
    treeState.addParameterListener (trimID, this);
}

JustaFilterAudioProcessor::~JustaFilterAudioProcessor()
{
    treeState.removeParameterListener (filterTypeID, this);
    treeState.removeParameterListener (bandwidthTypeID, this);
    treeState.removeParameterListener (cutoffID, this);
    treeState.removeParameterListener (bandwidthID, this);
    treeState.removeParameterListener (gainID, this);
    treeState.removeParameterListener (qualityID, this);
    treeState.removeParameterListener (phaseID, this);
    treeState.removeParameterListener (trimID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout JustaFilterAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

  // Make sure to update the number of reservations after adding params
  params.reserve(7);
    
    juce::StringArray filterTypes =
    {
        "Low Shelf",
        "Low Cut",
        "Notch",
        "High Cut",
        "High Shelf"
    };
    
    juce::StringArray qTypes =
    {
        "Linear Q",
        "Analog Q"
    };
    
    juce::StringArray os =
    {
        "No OS",
        "8X OS"
    };

  auto pFilterType = std::make_unique<juce::AudioParameterInt>(filterTypeID, filterTypeName, 0, 4, 0);
  auto pBandwithType = std::make_unique<juce::AudioParameterInt>(bandwidthTypeID, bandwidthTypeName, 0, 1, 0);
  auto pCutoff = std::make_unique<juce::AudioParameterFloat>(cutoffID, cutoffName, juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.3f, false), 1000.0f);
  auto pBandwith = std::make_unique<juce::AudioParameterFloat>(bandwidthID, bandwidthName, 0.05f, 0.95f, 0.33f);
  auto pQGain = std::make_unique<juce::AudioParameterFloat>(gainID, gainName, -12.0f, 12.0f, 0.0f);
  auto pQuality = std::make_unique<juce::AudioParameterInt>(qualityID, qualityName, 0, 1, 0);
  auto pPhase = std::make_unique<juce::AudioParameterBool>(phaseID, phaseName, false);
  auto pTrim = std::make_unique<juce::AudioParameterFloat>(trimID, trimName, -24.0f, 24.0f, 0.0f);

  params.push_back(std::move(pFilterType));
  params.push_back(std::move(pBandwithType));
  params.push_back(std::move(pCutoff));
  params.push_back(std::move(pBandwith));
  params.push_back(std::move(pQGain));
  params.push_back(std::move(pQuality));
  params.push_back(std::move(pPhase));
  params.push_back(std::move(pTrim));

  return { params.begin(), params.end() };
}

void JustaFilterAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    
    if (parameterID == filterTypeID)
    {
        filterModule.setParameter(LV_SVFilter::ParameterId::kType, static_cast<int>(newValue));
    }
    
    if (parameterID == bandwidthTypeID)
    {
        filterModule.setParameter(LV_SVFilter::ParameterId::kQType, newValue);
    }
    
    if (parameterID == cutoffID)
    {
        filterModule.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
    }
    
    if (parameterID == bandwidthID)
    {
        filterModule.setParameter(LV_SVFilter::ParameterId::kQ, newValue);
    }
    
    if (parameterID == gainID)
    {
        filterModule.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
    }
    
    /** Oversampling */
    if (parameterID == qualityID)
    {
        if (newValue == 0)
        {
            oversamplingState = false;
            overSampleRate = getSampleRate();
        }
            
        else
        {
            oversamplingState = true;
            overSampleRate = getSampleRate() * oversamplingModule.getOversamplingFactor();
        }
    }
    
    if (parameterID == phaseID)
    {
        checkPhase(newValue);
    }
    
    if (parameterID == trimID)
    {
        trimModule.setGainDecibels(newValue);
    }
}

void JustaFilterAudioProcessor::checkPhase(const float p)
{
    bool newPhase = static_cast<bool>(p);
    phaseState = !newPhase;
}

//==============================================================================
const juce::String JustaFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JustaFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JustaFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JustaFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JustaFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JustaFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JustaFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JustaFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JustaFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void JustaFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JustaFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Init Spec
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    // Init Filter
    filterModule.prepare(spec);
    initFilter();
    
    // Init OS
    oversamplingState = *treeState.getRawParameterValue(qualityID);

    if (oversamplingState)
    {
        overSampleRate = spec.sampleRate * oversamplingModule.getOversamplingFactor();
    }

    else
    {
        overSampleRate = spec.sampleRate;
    }

    projectSampleRate = sampleRate;
    
    oversamplingModule.reset();
    oversamplingModule.initProcessing(samplesPerBlock);
    
    // Init phase
    checkPhase(*treeState.getRawParameterValue(phaseID));
    
    trimModule.prepare(spec);
    trimModule.setRampDurationSeconds(0.02);
    
    lfoGenerator.prepare(spec);
    lfoGenerator.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, 4);
}

void JustaFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JustaFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JustaFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> audioBlock {buffer};
    juce::dsp::AudioBlock<float> upSampledBlock;
    
    for (int sample = 0; sample < audioBlock.getNumSamples(); ++sample)
    {
        lfoGenerator.process();
    }
    
    for (int ch = 0; ch < audioBlock.getNumChannels(); ++ch)
    {
        for (int sample = 0; sample < audioBlock.getNumSamples(); ++sample)
        {
            float* data = audioBlock.getChannelPointer(ch);
            
            
            trimModule.setGainDecibels(lfoGenerator.getCurrentLFOValue() * 6.0);
            data[sample] = trimModule.processSample(data[sample]);
        }
    }
    
//    if (oversamplingState)
//    {
//        upSampledBlock = oversamplingModule.processSamplesUp(audioBlock);
//        filterModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, overSampleRate);
//        filterModule.processBlock(upSampledBlock);
//        oversamplingModule.processSamplesDown(audioBlock);
//    }
//
//    else
//    {
//        filterModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, getSampleRate());
//        filterModule.processBlock(audioBlock);
//        lfoGenerator.process();
//    }
//
//    if (phaseState)
//    {
//        buffer.applyGain(-1.0f);
//    }
//
//    trimModule.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

//==============================================================================
bool JustaFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JustaFilterAudioProcessor::createEditor()
{
    return new JustaFilterAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void JustaFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void JustaFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        treeState.state = tree;
        
        // Window Size
        windowWidth = variableTree.getProperty("width");
        windowHeight = variableTree.getProperty("height");
        
        //Filter params
        initFilter();
        
        oversamplingState = *treeState.getRawParameterValue(qualityID) - 1;
        
        checkPhase(*treeState.getRawParameterValue(phaseID));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JustaFilterAudioProcessor();
}
