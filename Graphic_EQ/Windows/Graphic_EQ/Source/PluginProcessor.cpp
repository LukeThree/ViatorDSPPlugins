/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicEQAudioProcessor::GraphicEQAudioProcessor()
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
    treeState.addParameterListener (oneId, this);
    treeState.addParameterListener (twoId, this);
    treeState.addParameterListener (threeId, this);
    treeState.addParameterListener (fourId, this);
    treeState.addParameterListener (fiveId, this);
    treeState.addParameterListener (sixId, this);
    treeState.addParameterListener (sevenId, this);
    treeState.addParameterListener (eightId, this);
    treeState.addParameterListener (nineId, this);
    treeState.addParameterListener (tenId, this);

    treeState.addParameterListener (trimSliderId, this);
    treeState.addParameterListener (driveId, this);
    
    treeState.addParameterListener (phaseId, this);
    treeState.addParameterListener (lowBellId, this);
    treeState.addParameterListener (highBellId, this);
}

GraphicEQAudioProcessor::~GraphicEQAudioProcessor()
{
    treeState.removeParameterListener (oneId, this);
    treeState.removeParameterListener (twoId, this);
    treeState.removeParameterListener (threeId, this);
    treeState.removeParameterListener (fourId, this);
    treeState.removeParameterListener (fiveId, this);
    treeState.removeParameterListener (sixId, this);
    treeState.removeParameterListener (sevenId, this);
    treeState.removeParameterListener (eightId, this);
    treeState.removeParameterListener (nineId, this);
    treeState.removeParameterListener (tenId, this);

    treeState.removeParameterListener (trimSliderId, this);
    treeState.removeParameterListener (driveId, this);

    treeState.removeParameterListener (phaseId, this);
    treeState.removeParameterListener (lowBellId, this);
    treeState.removeParameterListener (highBellId, this);

}

//==============================================================================
const juce::String GraphicEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GraphicEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GraphicEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GraphicEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GraphicEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GraphicEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GraphicEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GraphicEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GraphicEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void GraphicEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GraphicEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    // Set the sample rate and use it for dsp modules
    lastSampleRate = sampleRate;
    
    // Initialize global variables to corresponding params to make sure they load properly
    setPhase(*treeState.getRawParameterValue(phaseId));
    
    lowBellState = *treeState.getRawParameterValue(lowBellId);
    highBellState = *treeState.getRawParameterValue(highBellId);
    
    rawDrive = *treeState.getRawParameterValue(driveId) + 4.0f;
    
    // Make sure to prepare and reset all dsp modules
    for (auto i = 0; i < filters.size(); ++i) {
        filters[i]->reset();
        filters[i]->prepare(spec);
    }
    
//    disLP.reset();
//    disLP.prepare(spec);
//    
//    disHS1.reset();
//    disHS1.prepare(spec);
//    
//    disHS2.reset();
//    disHS2.prepare(spec);
//    
//    disHS3.reset();
//    disHS3.prepare(spec);
    
    // Initialize all filter at program start to the values saved in the value tree
    updateFilter1(*treeState.getRawParameterValue(oneId));
    updateFilter2(*treeState.getRawParameterValue(twoId), *treeState.getRawParameterValue(lowBellId));
    updateFilter3(*treeState.getRawParameterValue(threeId));
    updateFilter4(*treeState.getRawParameterValue(fourId));
    updateFilter5(*treeState.getRawParameterValue(fiveId));
    updateFilter6(*treeState.getRawParameterValue(sixId));
    updateFilter7(*treeState.getRawParameterValue(sevenId));
    updateFilter8(*treeState.getRawParameterValue(eightId));
    updateFilter9(*treeState.getRawParameterValue(nineId), *treeState.getRawParameterValue(highBellId));
    updateFilter10(*treeState.getRawParameterValue(tenId));
    
    // Filters for waveshaping
    //*disLP.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, 900.0f);
    //*disHS1.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 2100.0f, 1.0f, 21.0f);
    //*disHS2.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 9000.0f, 1.0f, 21.0f);
    //*disHS3.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 18000.0f, 1.0f, 21.0f);
    
    // Initialize trim gain
    lastTrimGain = pow(10, *treeState.getRawParameterValue(trimSliderId) / 20);
    currentTrimGain = pow(10, *treeState.getRawParameterValue(trimSliderId) / 20);
    
}

void GraphicEQAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GraphicEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GraphicEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Audio block for dsp modules
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    
    // Process filters
    for (auto i = 0; i < filters.size(); ++i)
    {
        filters[i]->process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }
    
    // Main process loop
    callProcessorCore(buffer, buffer.getNumChannels());
    
    //disLP.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //disHS1.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //disHS2.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //disHS3.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

juce::AudioProcessorValueTreeState::ParameterLayout GraphicEQAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    // Make sure to update the number of reservations after adding params
    params.reserve(15);
    
    auto mBand1 = std::make_unique<juce::AudioParameterFloat>(oneId, oneName, -12.0f, 12.0f, 0.0f);
    auto mBand2 = std::make_unique<juce::AudioParameterFloat>(twoId, twoName, -12.0f, 12.0f, 0.0f);
    auto mBand3 = std::make_unique<juce::AudioParameterFloat>(threeId, threeName, -12.0f, 12.0f, 0.0f);
    auto mBand4 = std::make_unique<juce::AudioParameterFloat>(fourId, fourName, -12.0f, 12.0f, 0.0f);
    auto mBand5 = std::make_unique<juce::AudioParameterFloat>(fiveId, fiveName, -12.0f, 12.0f, 0.0f);
    auto mBand6 = std::make_unique<juce::AudioParameterFloat>(sixId, sixName, -12.0f, 12.0f, 0.0f);
    auto mBand7 = std::make_unique<juce::AudioParameterFloat>(sevenId, sevenName, -12.0f, 12.0f, 0.0f);
    auto mBand8 = std::make_unique<juce::AudioParameterFloat>(eightId, eightName, -12.0f, 12.0f, 0.0f);
    auto mBand9 = std::make_unique<juce::AudioParameterFloat>(nineId, nineName, -12.0f, 12.0f, 0.0f);
    auto mBand10 = std::make_unique<juce::AudioParameterFloat>(tenId, tenName, -12.0f, 12.0f, 0.0f);
    
    auto mDrive = std::make_unique<juce::AudioParameterFloat>(driveId, driveName, 0.0f, 10.0f, 0.0f);
    auto mTrim = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -24.0f, 24.0f, 0.0f);
    
    auto mPhase = std::make_unique<juce::AudioParameterInt>(phaseId, phaseName, 0, 1, 0);
    auto mLowBell = std::make_unique<juce::AudioParameterInt>(lowBellId, lowBellName, 0, 1, 1);
    auto mHighBell = std::make_unique<juce::AudioParameterInt>(highBellId, highBellName, 0, 1, 1);


    params.push_back(std::move(mBand1));
    params.push_back(std::move(mBand2));
    params.push_back(std::move(mBand3));
    params.push_back(std::move(mBand4));
    params.push_back(std::move(mBand5));
    params.push_back(std::move(mBand6));
    params.push_back(std::move(mBand7));
    params.push_back(std::move(mBand8));
    params.push_back(std::move(mBand9));
    params.push_back(std::move(mBand10));

    params.push_back(std::move(mDrive));
    params.push_back(std::move(mTrim));
    
    params.push_back(std::move(mPhase));
    params.push_back(std::move(mLowBell));
    params.push_back(std::move(mHighBell));
    
    return { params.begin(), params.end() };
}

void GraphicEQAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue){
    
    // Update params here
    
     if (parameterID == trimSliderId) {
        
        currentTrimGain = pow(10, newValue / 20);
        
    } else  if (parameterID == phaseId) {
        
        setPhase(newValue);
        
    } else if (parameterID == oneId){
        
        updateFilter1(newValue);
        
    } else if (parameterID == twoId){
        
        updateFilter2(newValue, lowBellState);
        filter2Gain = newValue;
        
    } else if (parameterID == threeId){
        
        updateFilter3(newValue);
        
    } else if (parameterID == fourId){
        
        updateFilter4(newValue);
        
    } else if (parameterID == fiveId){
        
        updateFilter5(newValue);
        
    } else if (parameterID == sixId){
        
        updateFilter6(newValue);
        
    } else if (parameterID == sevenId){
        
        updateFilter7(newValue);
        
    } else if (parameterID == eightId){
        
        updateFilter8(newValue);
        
    } else if (parameterID == nineId){
        
        updateFilter9(newValue, highBellState);
        filter9Gain = newValue;
        
    } else if (parameterID == tenId){
        
        updateFilter10(newValue);
        
    } else if (parameterID == highBellId){
        
        highBellState = newValue;
        updateFilter9(filter9Gain, highBellState);
        
    } else if (parameterID == lowBellId){
        
        lowBellState = newValue;
        updateFilter2(filter2Gain, lowBellState);
        
    } else if (parameterID == driveId){
        
        rawDrive = newValue + 4.0f;
    }
}

//==============================================================================
bool GraphicEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GraphicEQAudioProcessor::createEditor()
{
    return new GraphicEQAudioProcessorEditor (*this);
}

//==============================================================================
void GraphicEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void GraphicEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GraphicEQAudioProcessor();
}

void GraphicEQAudioProcessor::setPhase(int phase)
{
    if (phase == 0){
        
        currentPhase = 1;
        
    } else {
        
        currentPhase = -1;
    }
}
