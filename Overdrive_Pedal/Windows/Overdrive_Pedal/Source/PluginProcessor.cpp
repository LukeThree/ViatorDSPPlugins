/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdrivePedalAudioProcessor::OverdrivePedalAudioProcessor()
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
oversamplingProcessor (2, 1, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR, true, false)
#endif
{
    treeState.addParameterListener (inputSliderId, this);
    treeState.addParameterListener (driveSliderId, this);
    treeState.addParameterListener (toneSliderId, this);
    treeState.addParameterListener (trimSliderId, this);
}

OverdrivePedalAudioProcessor::~OverdrivePedalAudioProcessor()
{
    treeState.removeParameterListener (inputSliderId, this);
    treeState.removeParameterListener (driveSliderId, this);
    treeState.removeParameterListener (toneSliderId, this);
    treeState.removeParameterListener (trimSliderId, this);
}

//==============================================================================
const juce::String OverdrivePedalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OverdrivePedalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OverdrivePedalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OverdrivePedalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OverdrivePedalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OverdrivePedalAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OverdrivePedalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OverdrivePedalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OverdrivePedalAudioProcessor::getProgramName (int index)
{
    return {};
}

void OverdrivePedalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OverdrivePedalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    lastSampleRate = sampleRate;
    
    
    prepareFilters(spec);
    prepareGains(spec);
    prepareDrive();
    
    oversamplingProcessor.initProcessing(spec.maximumBlockSize);
    oversamplingProcessor.reset();
}

void OverdrivePedalAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OverdrivePedalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void OverdrivePedalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    const auto numChannels = buffer.getNumChannels();
    
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    
    // Input gain and smoothing
    if (currentPreampGain == lastPreampGain)
    {
        buffer.applyGain(0, buffer.getNumSamples(), currentPreampGain);

    } else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), lastPreampGain, currentPreampGain);
        lastPreampGain = currentPreampGain;
    }
    
    // Pre distortion DSP
    callPreOSChain(audioBlock);
            
    auto upSampledBlock = oversamplingProcessor.processSamplesUp(audioBlock);
    
    // Main loop with distortion
    callProcessorCore(upSampledBlock, numChannels);
    
    oversamplingProcessor.processSamplesDown(audioBlock);
    
    // Post distortion DSP
    callPostOSChain(audioBlock);
    
    // Trim gain and smoothing
    if (currentTrimGain == lastTrimGain)
    {
        buffer.applyGain(0, buffer.getNumSamples(), currentTrimGain);

    } else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), lastTrimGain, currentTrimGain);
        lastTrimGain = currentTrimGain;
    }
}

//==============================================================================
bool OverdrivePedalAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdrivePedalAudioProcessor::createEditor()
{
    return new OverdrivePedalAudioProcessorEditor (*this);
}

//==============================================================================
void OverdrivePedalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void OverdrivePedalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
        if (tree.isValid())
        {
            treeState.state = tree;
        }
}

juce::AudioProcessorValueTreeState::ParameterLayout OverdrivePedalAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(4);
    
    auto inputParam = std::make_unique<juce::AudioParameterFloat>(inputSliderId, inputSliderName, -12.0f, 12.0f, 0.0f);
    auto driveParam = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0f, 12.0f, 0.0f);
    auto toneParam = std::make_unique<juce::AudioParameterFloat>(toneSliderId, toneSliderName, -6.0f, 6.0f, 0.0f);
    auto trimParam = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -24.0f, 24.0f, 0.0f);

    params.push_back(std::move(inputParam));
    params.push_back(std::move(driveParam));
    params.push_back(std::move(toneParam));
    params.push_back(std::move(trimParam));
    
    return { params.begin(), params.end() };
}

void OverdrivePedalAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue){
    
    if (parameterID == toneSliderId) {
        
        updateHSFilter(pow(10, newValue / 2 / 20));
        lpFilter.setCutoffFrequencyHz(pow(10, newValue / 20) * lpFilterCutoff);
        
    } else if (parameterID == trimSliderId){
        
        currentTrimGain = pow(10, newValue / 20);
        
    } else if (parameterID == driveSliderId){
        
        updateDrive(newValue);
        
    } else {
        currentPreampGain = pow(10, newValue / 20);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OverdrivePedalAudioProcessor();
}
