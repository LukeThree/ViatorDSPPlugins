/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbAudioProcessor::ReverbAudioProcessor()
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
    treeState.addParameterListener (filterEngageId, this);
    treeState.addParameterListener (filterModeId, this);
    treeState.addParameterListener (cutoffSliderId, this);
    treeState.addParameterListener (resonanceSliderId, this);
    treeState.addParameterListener (driveSliderId, this);
    treeState.addParameterListener (roomSizeSliderId, this);
    treeState.addParameterListener (dampingSliderId, this);
    treeState.addParameterListener (widthSliderId, this);
    treeState.addParameterListener (drySliderId, this);
    treeState.addParameterListener (wetSliderId, this);
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
    treeState.removeParameterListener (filterEngageId, this);
    treeState.removeParameterListener (filterModeId, this);
    treeState.removeParameterListener (cutoffSliderId, this);
    treeState.removeParameterListener (resonanceSliderId, this);
    treeState.removeParameterListener (driveSliderId, this);
    treeState.removeParameterListener (roomSizeSliderId, this);
    treeState.removeParameterListener (dampingSliderId, this);
    treeState.removeParameterListener (widthSliderId, this);
    treeState.removeParameterListener (drySliderId, this);
    treeState.removeParameterListener (wetSliderId, this);
}

//==============================================================================
const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    ladderProcessor.prepare(spec);
    ladderProcessor.reset();
    ladderProcessor.setCutoffFrequencyHz(*treeState.getRawParameterValue(cutoffSliderId));
    ladderProcessor.setResonance(*treeState.getRawParameterValue(resonanceSliderId) * 0.01);
    ladderProcessor.setDrive(pow(10, *treeState.getRawParameterValue(driveSliderId) / 20));
    
    if (*treeState.getRawParameterValue(filterEngageId) == 0){
        ladderProcessor.setEnabled(false);
    } else {
        ladderProcessor.setEnabled(true);
    }
    
    if (*treeState.getRawParameterValue(filterModeId) == 0){
        ladderProcessor.setMode(juce::dsp::LadderFilterMode::LPF12);
    } else {
        ladderProcessor.setMode(juce::dsp::LadderFilterMode::LPF24);
    }
    
    reverbProcessor.prepare(spec);
    reverbProcessor.reset();
    reverbParams.roomSize = *treeState.getRawParameterValue(roomSizeSliderId) * .01;
    reverbParams.damping = *treeState.getRawParameterValue(dampingSliderId) * .01;
    reverbParams.width = *treeState.getRawParameterValue(widthSliderId) * .01;
    reverbParams.dryLevel = *treeState.getRawParameterValue(drySliderId) * .01;
    reverbParams.wetLevel = *treeState.getRawParameterValue(wetSliderId) * .01;
    reverbProcessor.setParameters(reverbParams);
}

void ReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> audioBlock {buffer};

    ladderProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    reverbProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

}

//==============================================================================
bool ReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbAudioProcessor::createEditor()
{
    return new ReverbAudioProcessorEditor (*this);
}

//==============================================================================
void ReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void ReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
        
        if (tree.isValid())
        {
            treeState.state = tree;
        }
}

void ReverbAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue){
    if (parameterID == cutoffSliderId){
        ladderProcessor.setCutoffFrequencyHz(newValue);
    } else if (parameterID == resonanceSliderId){
        ladderProcessor.setResonance(newValue * 0.01);
    } else if (parameterID == driveSliderId){
        ladderProcessor.setDrive(pow(10, newValue / 20));
    } else if (parameterID == filterEngageId){
        
        if (newValue == 0){
            ladderProcessor.setEnabled(false);
        } else {
            ladderProcessor.setEnabled(true);
        }
    } else if (parameterID == filterModeId){
        if (newValue == 0){
            ladderProcessor.setMode(juce::dsp::LadderFilterMode::LPF12);
        } else {
            ladderProcessor.setMode(juce::dsp::LadderFilterMode::LPF24);
        }
    } else if (parameterID == wetSliderId){
        reverbParams.wetLevel = newValue * 0.01;
        reverbProcessor.setParameters(reverbParams);
    } else if (parameterID == drySliderId){
        reverbParams.dryLevel = newValue * 0.01;
        reverbProcessor.setParameters(reverbParams);
    } else if (parameterID == roomSizeSliderId){
        reverbParams.roomSize = newValue * 0.01;
        reverbProcessor.setParameters(reverbParams);
    } else if (parameterID == dampingSliderId){
        reverbParams.damping = newValue * 0.01;
        reverbProcessor.setParameters(reverbParams);
    } else {
        reverbParams.width = newValue * 0.01;
        reverbProcessor.setParameters(reverbParams);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout ReverbAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(10);
    
    auto filterEngageParam = std::make_unique<juce::AudioParameterInt>(filterEngageId, filterEngageName, 0, 1, 1);
    auto filterModeParam = std::make_unique<juce::AudioParameterInt>(filterModeId, filterModeName, 0, 1, 0);
    auto cutoffParam = std::make_unique<juce::AudioParameterInt>(cutoffSliderId, cutoffSliderName, 20, 20000, 1200);
    auto resonanceParam = std::make_unique<juce::AudioParameterInt>(resonanceSliderId, resonanceSliderName, 0, 100, 0);
    auto driveParam = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0f, 24.0f, 0.0f);
    auto roomSizeParam = std::make_unique<juce::AudioParameterInt>(roomSizeSliderId, roomSizeSliderName, 0, 100, 50);
    auto dampingParam = std::make_unique<juce::AudioParameterInt>(dampingSliderId, dampingSliderName, 0, 100, 50);
    auto widthParam = std::make_unique<juce::AudioParameterInt>(widthSliderId, widthSliderName, 0, 100, 100);
    auto dryParam = std::make_unique<juce::AudioParameterInt>(drySliderId, drySliderName, 0, 100, 0);
    auto wetParam = std::make_unique<juce::AudioParameterInt>(wetSliderId, wetSliderName, 0, 100, 50);
    
    params.push_back(std::move(filterEngageParam));
    params.push_back(std::move(filterModeParam));
    params.push_back(std::move(cutoffParam));
    params.push_back(std::move(resonanceParam));
    params.push_back(std::move(driveParam));
    params.push_back(std::move(roomSizeParam));
    params.push_back(std::move(dampingParam));
    params.push_back(std::move(widthParam));
    params.push_back(std::move(dryParam));
    params.push_back(std::move(wetParam));
    
    return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}
