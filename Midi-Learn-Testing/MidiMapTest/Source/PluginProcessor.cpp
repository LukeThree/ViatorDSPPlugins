/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiMapTestAudioProcessor::MidiMapTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("p1", this);
    treeState.addParameterListener("p2", this);
    treeState.addParameterListener("p3", this);
    treeState.addParameterListener("p4", this);
    treeState.addParameterListener("macrodial", this);
}

MidiMapTestAudioProcessor::~MidiMapTestAudioProcessor()
{
    treeState.removeParameterListener("p1", this);
    treeState.removeParameterListener("p2", this);
    treeState.removeParameterListener("p3", this);
    treeState.removeParameterListener("p4", this);
    treeState.removeParameterListener("macrodial", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MidiMapTestAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
    auto p1 = std::make_unique<juce::AudioParameterFloat>("p1", "P1", 0.0f, 10.0f, 0.0f);
    auto p2 = std::make_unique<juce::AudioParameterFloat>("p2", "P2", 0.0f, 10.0f, 0.0f);
    auto p3 = std::make_unique<juce::AudioParameterFloat>("p3", "P3", 0.0f, 10.0f, 0.0f);
    auto p4 = std::make_unique<juce::AudioParameterFloat>("p4", "P4", 0.0f, 10.0f, 0.0f);
    auto pMacroDial = std::make_unique<juce::AudioParameterFloat>("macrodial", "Macro Dial", 0.0f, 10.0f, 0.0f);
    auto learnButton = std::make_unique<juce::AudioParameterBool>("learn", "Learn", false);
    
    params.push_back(std::move(p1));
    params.push_back(std::move(p2));
    params.push_back(std::move(p3));
    params.push_back(std::move(p4));
    params.push_back(std::move(pMacroDial));
    params.push_back(std::move(learnButton));
    
    return { params.begin(), params.end() };
    
}

void MidiMapTestAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (macroMap.getMacro1Map().size() > 0)
    {
        for (size_t i = 0; i < macroMap.getMacro1Map().size(); ++i)
        {
            treeState.getParameterAsValue(macroMap.getMacro1Map()[i]) = treeState.getRawParameterValue("macrodial")->load();
            DBG(macroMap.getMacro1Map()[i]);
        }
    }
}

//==============================================================================
const juce::String MidiMapTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiMapTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiMapTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiMapTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiMapTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiMapTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiMapTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiMapTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiMapTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiMapTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidiMapTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiMapTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiMapTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MidiMapTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (auto m : midiMessages)
    {
        if (m.getMessage().isController())
        {
            auto midiChannel = m.getMessage().getChannel();
            auto ccNumber = m.getMessage().getControllerNumber();
            
            midiMap.setCurrentValues(midiChannel, ccNumber);
            
            auto ccValue  = juce::jmap(static_cast<float>(m.getMessage().getControllerValue()), 0.0f, 127.0f, 0.0f, 10.0f);
            
            for (const auto& item : midiMap.midiVector)
            {
                if (midiChannel == item.channel && ccNumber == item.cc)
                {
                    treeState.getParameterAsValue(item.parameter) = ccValue;
                }
            }
        }
    }
}

//==============================================================================
bool MidiMapTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiMapTestAudioProcessor::createEditor()
{
    return new MidiMapTestAudioProcessorEditor (*this);
}

//==============================================================================
void MidiMapTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    /** MIDI */
    treeState.state.appendChild(midiMap.midiChannelsTree, nullptr);
    treeState.state.appendChild(midiMap.midiCCTree, nullptr);
    treeState.state.appendChild(midiMap.midiParametersTree, nullptr);
    
    /** MACRO */
    treeState.state.appendChild(macroMap.macroParametersTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void MidiMapTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    
    /** MIDI */
    midiMap.midiChannelsTree = tree.getChildWithName("MidiChannels");
    midiMap.midiCCTree = tree.getChildWithName("MidiCCs");
    midiMap.midiParametersTree = tree.getChildWithName("MidiParameters");
    
    /** MACRO */
    macroMap.macroParametersTree = tree.getChildWithName("MacroParameters");
    
    if (tree.isValid())
    {
        treeState.state = tree;
        
        // Recall midi mappings
        for (int i = 0; i < midiMap.midiChannelsTree.getNumProperties(); i++)
        {
            auto channel = midiMap.midiChannelsTree.getProperty(midiMap.midiChannelsTree.getPropertyName(i));

            auto cc = midiMap.midiCCTree.getProperty(midiMap.midiCCTree.getPropertyName(i));

            auto param = midiMap.midiParametersTree.getProperty(midiMap.midiParametersTree.getPropertyName(i));

            midiMap.setMidiGroup(channel, cc, param);
        }
        
        for (int i = 0; i < macroMap.macroParametersTree.getNumProperties(); i++)
        {
            auto macro1Params = macroMap.macroParametersTree.getProperty(macroMap.macroParametersTree.getPropertyName(i));
            
            macroMap.setMacro1Group(macro1Params);
        }
    }
    
    else
    {
        DBG("The tree is invalid for some reason!");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiMapTestAudioProcessor();
}
