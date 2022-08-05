/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
GainStagerAudioProcessor::GainStagerAudioProcessor()
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
    treeState.addParameterListener (preampSliderId, this);
    treeState.addParameterListener (phaseId, this);
    treeState.addParameterListener (listenId, this);
    treeState.addParameterListener (stageTypeId, this);
    treeState.addParameterListener (calibrationId, this);

}

GainStagerAudioProcessor::~GainStagerAudioProcessor()
{
    treeState.removeParameterListener (preampSliderId, this);
    treeState.removeParameterListener (phaseId, this);
    treeState.removeParameterListener (listenId, this);
    treeState.removeParameterListener (stageTypeId, this);
    treeState.removeParameterListener (calibrationId, this);

}

//==============================================================================
const juce::String GainStagerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainStagerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainStagerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainStagerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainStagerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainStagerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainStagerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainStagerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainStagerAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainStagerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainStagerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    // Set the sample rate and use it for dsp modules
    currentSampleRate = sampleRate;
    
    // Initialize global variables to corresponding params to make sure they load properly
    setPhase(*treeState.getRawParameterValue(phaseId));
    
    // Make sure to prepare and reset all dsp modules
    preampProcessor.reset();
    preampProcessor.prepare(spec);
    preampProcessor.setGainDecibels(*treeState.getRawParameterValue(preampSliderId));
    
    stageType = *treeState.getRawParameterValue(stageTypeId);
    
    tempAvg = variableTree.getProperty("temp average");
    tempPeak = variableTree.getProperty("temp peak");
    
    switch (int(*treeState.getRawParameterValue(calibrationId))) {
            
        case 0: thresh = -18.0f; break;
        case 1: thresh = -21.0f; break;
        case 2: thresh = -15.0f; break;
        case 3: thresh = -12.0f; break;

    }
}

void GainStagerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainStagerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GainStagerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    /*=========================================================================================================*/

    juce::dsp::AudioBlock<float> audioBlock {buffer};

    if (listen)
    {

        if (loopCount < currentSampleRate * n)
        {

            for (auto channel{ 0 }; channel < buffer.getNumChannels(); channel++)
            {
                for (auto sample{ 0 }; sample < buffer.getNumSamples(); sample++)
                {
                    // If they listen in silence, it won't blow their speakers
                    // Also, amps could cause very low feedback and also mess it up!
                    if (abs(buffer.getSample(channel, sample)) > 0.001)
                    {
                        if (juce::Decibels::gainToDecibels(abs(buffer.getSample(channel, sample))) > -60)
                        {
                            avgVector.push_back(abs(buffer.getSample(channel, sample)));
                            loopCount++;
                        }
                        
                        else
                        {
                            loopCount++;
                        }
                    }
                }
            }
        }

        else
        {
            //Perfect for very consistent signals like oscillators or heavily limited stuff
            tempPeak = getPeakSignal(avgVector, avgRMS);
            variableTree.setProperty("temp peak", tempPeak, nullptr);
                
            //Great for Dynamic material, stuff where the meter is jumping around
            tempAvg = getAverageSignal(avgVector, avgRMS);
            variableTree.setProperty("temp average", tempAvg, nullptr);
                        
            // See which one the user wants
            switch (stageType) {
                    
                case 0: treeState.getParameterAsValue(preampSliderId) = thresh - tempAvg - 4.5f; break;
                case 1: treeState.getParameterAsValue(preampSliderId) = thresh - tempPeak; break;
            }
            
            listen = false;
        }
    }
    
    preampProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}



//==============================================================================
bool GainStagerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainStagerAudioProcessor::createEditor()
{
    return new GainStagerAudioProcessorEditor (*this);
}

//==============================================================================
void GainStagerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    treeState.state.appendChild(variableTree, nullptr);

    juce::MemoryOutputStream stream(destData, false);
    
    treeState.state.writeToStream (stream);
}

void GainStagerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    
    variableTree = tree.getChildWithName("Variables");
    
    treeState.state = tree;
    
    windowWidth = variableTree.getProperty("width");
    windowHeight = variableTree.getProperty("height");
    
    // Initialize global variables to corresponding params to make sure they load properly
    setPhase(*treeState.getRawParameterValue(phaseId));
    
    preampProcessor.setGainDecibels(*treeState.getRawParameterValue(preampSliderId));
    
    stageType = *treeState.getRawParameterValue(stageTypeId);
    
    tempAvg = variableTree.getProperty("temp average");
    tempPeak = variableTree.getProperty("temp peak");
    
    switch (int(*treeState.getRawParameterValue(calibrationId))) {
            
        case 0: thresh = -18.0f; break;
        case 1: thresh = -21.0f; break;
        case 2: thresh = -15.0f; break;
        case 3: thresh = -12.0f; break;

    }
}

float GainStagerAudioProcessor::getGain(float value)
{
    return pow(10.0f, value / 20.0f);
}

void GainStagerAudioProcessor::setPhase(int phase)
{
    if (phase == 0){
        
        currentPhase = 1;
        
    } else {
        
        currentPhase = -1;
    }
}

float GainStagerAudioProcessor::roundToTwo(const float input)
{
    return round(input * 100.0f) / 100.0f;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainStagerAudioProcessor();
}
