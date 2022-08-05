/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoxDriveAudioProcessor::VoxDriveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
, oversamplingModule(2, 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR)
#endif
{
    treeState.addParameterListener(inputID, this);
    treeState.addParameterListener(cutoffID, this);
    treeState.addParameterListener(mixID, this);
    treeState.addParameterListener(lowpassID, this);
    treeState.addParameterListener(trimID, this);
    treeState.addParameterListener(outputID, this);
    treeState.addParameterListener(phaseID, this);
    treeState.addParameterListener(hqID, this);
    
    variableTree.setProperty("mastercolor", juce::Colours::black.toString(), nullptr);
    variableTree.setProperty("alpha", false, nullptr);
}

VoxDriveAudioProcessor::~VoxDriveAudioProcessor()
{
    treeState.removeParameterListener(inputID, this);
    treeState.removeParameterListener(cutoffID, this);
    treeState.removeParameterListener(mixID, this);
    treeState.removeParameterListener(lowpassID, this);
    treeState.removeParameterListener(trimID, this);
    treeState.removeParameterListener(outputID, this);
    treeState.removeParameterListener(phaseID, this);
    treeState.removeParameterListener(hqID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout VoxDriveAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
    auto pInput = std::make_unique<juce::AudioParameterFloat>(inputID, inputName, 0.0f, 50.0f, 0.0f);
    auto pCutoff = std::make_unique<juce::AudioParameterFloat>(cutoffID, cutoffName, juce::NormalisableRange<float>(500.0f, 20000.0f, 1.0f, 0.5), 500.0f);
    auto pMix = std::make_unique<juce::AudioParameterInt>(mixID, mixName, 0, 100, 100);
    auto pLowpass = std::make_unique<juce::AudioParameterFloat>(lowpassID, lowpassName, juce::NormalisableRange<float>(1000.0f, 20000.0f, 1.0f, 0.5), 20000.0f);
    auto pTrim = std::make_unique<juce::AudioParameterFloat>(trimID, trimName, -24.0f, 24.0f, 0.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -24.0f, 24.0f, 0.0f);
    auto pPhase = std::make_unique<juce::AudioParameterBool>(phaseID, phaseName, false);
    auto pHQ = std::make_unique<juce::AudioParameterBool>(hqID, hqName, false);
    
    params.push_back(std::move(pInput));
    params.push_back(std::move(pCutoff));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pLowpass));
    params.push_back(std::move(pTrim));
    params.push_back(std::move(pOutput));
    params.push_back(std::move(pPhase));
    params.push_back(std::move(pHQ));
    
    return { params.begin(), params.end() };
}

void VoxDriveAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == hqID)
    {
        // Adjust samplerate of filters when oversampling
        if (treeState.getRawParameterValue(hqID)->load())
        {
            spec.sampleRate = getSampleRate() * oversamplingModule.getOversamplingFactor();
            lowShelf.prepare(spec);
            voxDistortionModule.prepare(spec);
            outModule.prepare(spec);
        }

        else
        {
            spec.sampleRate = getSampleRate();
            lowShelf.prepare(spec);
            voxDistortionModule.prepare(spec);
            outModule.prepare(spec);
        }
    }
    
    updateParameters();
}

void VoxDriveAudioProcessor::updateParameters()
{
    lowShelf.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 200.0f);
    lowShelf.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, 0.3f);
    lowShelf.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    lowShelf.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kLowShelf);
    
    if (treeState.getRawParameterValue(inputID)->load() <= 24)
    {
        lowShelf.setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, treeState.getRawParameterValue(inputID)->load() * 0.5f - juce::jmap(treeState.getRawParameterValue(mixID)->load(), 0.0f, 100.0f, 12.0f, 0.0f));
    }
    
    else
    {
        lowShelf.setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, 12.0f - juce::jmap(treeState.getRawParameterValue(mixID)->load(), 0.0f, 100.0f, 12.0f, 0.0f));
    }
    
    
    voxDistortionModule.setDrive(treeState.getRawParameterValue(inputID)->load());
    voxDistortionModule.setCutoff(treeState.getRawParameterValue(cutoffID)->load());
    voxDistortionModule.setMix(treeState.getRawParameterValue(mixID)->load());
    voxDistortionModule.setLPCutoff(treeState.getRawParameterValue(lowpassID)->load());
    voxDistortionModule.setTrim(treeState.getRawParameterValue(trimID)->load() - treeState.getRawParameterValue(inputID)->load() * 0.5f);
    voxDistortionModule.setPhase(treeState.getRawParameterValue(phaseID)->load());
    
    outModule.setRampDurationSeconds(0.02);
    outModule.setGainDecibels(treeState.getRawParameterValue(outputID)->load());
}

//==============================================================================
const juce::String VoxDriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VoxDriveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VoxDriveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VoxDriveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VoxDriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VoxDriveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VoxDriveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VoxDriveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VoxDriveAudioProcessor::getProgramName (int index)
{
    return {};
}

void VoxDriveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VoxDriveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /** Oversampling */
    oversamplingModule.initProcessing(samplesPerBlock);
    oversamplingModule.reset();
    
    // Adjust samplerate of filters when oversampling
    if (treeState.getRawParameterValue(hqID)->load())
    {
        spec.sampleRate = getSampleRate() * oversamplingModule.getOversamplingFactor();
    }

    else
    {
        spec.sampleRate = getSampleRate();
    }
    
    // Initialize spec for dsp modules
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    lowShelf.prepare(spec);
    voxDistortionModule.prepare(spec);
    outModule.prepare(spec);
    updateParameters();
}

void VoxDriveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoxDriveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VoxDriveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;


    juce::dsp::AudioBlock<float> audioBlock {buffer};
    juce::dsp::AudioBlock<float> upSampledBlock (buffer);

    // Oversample if ON
    if (treeState.getRawParameterValue(hqID)->load())
    {
        upSampledBlock = oversamplingModule.processSamplesUp(audioBlock);
        lowShelf.process(juce::dsp::ProcessContextReplacing<float>(upSampledBlock));
        voxDistortionModule.process(juce::dsp::ProcessContextReplacing<float>(upSampledBlock));
        oversamplingModule.processSamplesDown(audioBlock);
        outModule.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }

    // Don't Oversample if OFF
    else
    {
        lowShelf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        voxDistortionModule.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        outModule.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }
}

//==============================================================================
bool VoxDriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VoxDriveAudioProcessor::createEditor()
{
    return new VoxDriveAudioProcessorEditor (*this);
}

//==============================================================================
void VoxDriveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void VoxDriveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
        variableTree.setProperty("alpha", false, nullptr);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoxDriveAudioProcessor();
}
