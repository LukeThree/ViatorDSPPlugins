/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LV50AAudioProcessor::LV50AAudioProcessor()
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
oversamplingProcessor (2, 3, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR, true, false)

#endif
{
    treeState.addParameterListener (driveSliderId, this);
    treeState.addParameterListener (trimSliderId, this);
    treeState.addParameterListener (phaseId, this);
    
    treeState.addParameterListener (gain1Id, this);
    treeState.addParameterListener (gain2Id, this);
    treeState.addParameterListener (gain3Id, this);
    treeState.addParameterListener (gain4Id, this);
    
    treeState.addParameterListener (freq1Id, this);
    treeState.addParameterListener (freq2Id, this);
    treeState.addParameterListener (freq3Id, this);
    treeState.addParameterListener (freq4Id, this);
    
    treeState.addParameterListener (lowShelfBId, this);
    treeState.addParameterListener (highShelfBId, this);
    
    treeState.addParameterListener (menuId, this);
    
    treeState.addParameterListener (filtersBId, this);
}

LV50AAudioProcessor::~LV50AAudioProcessor()
{
    treeState.removeParameterListener (driveSliderId, this);
    treeState.removeParameterListener (trimSliderId, this);
    treeState.removeParameterListener (phaseId, this);
    
    treeState.removeParameterListener (gain1Id, this);
    treeState.removeParameterListener (gain2Id, this);
    treeState.removeParameterListener (gain3Id, this);
    treeState.removeParameterListener (gain4Id, this);

    treeState.removeParameterListener (freq1Id, this);
    treeState.removeParameterListener (freq2Id, this);
    treeState.removeParameterListener (freq3Id, this);
    treeState.removeParameterListener (freq4Id, this);
    
    treeState.removeParameterListener (lowShelfBId, this);
    treeState.removeParameterListener (highShelfBId, this);
    
    treeState.removeParameterListener (menuId, this);
    
    treeState.removeParameterListener (filtersBId, this);
}

//==============================================================================
const juce::String LV50AAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LV50AAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LV50AAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LV50AAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LV50AAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LV50AAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LV50AAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LV50AAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LV50AAudioProcessor::getProgramName (int index)
{
    return {};
}

void LV50AAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    
}

//==============================================================================
void LV50AAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate * oversamplingProcessor.getOversamplingFactor();
    spec.numChannels = getTotalNumOutputChannels();
    
    // Set the sample rate and use it for dsp modules
    // We need both samplerates to switch between oversampling states
    lastSampleRate = spec.sampleRate;
    projectSampleRate = sampleRate;
    
    // Initialize global variables to corresponding params to make sure they load properly
    setPhase(*treeState.getRawParameterValue(phaseId));
    
    // Initialize preamp gain
    mRawDrive = *treeState.getRawParameterValue(driveSliderId);
        
    // Initialize trim gain
    lastTrimGain = pow(10, *treeState.getRawParameterValue(trimSliderId) / 20);
    currentTrimGain = pow(10, *treeState.getRawParameterValue(trimSliderId) / 20);
    
    // EQ Filters
    prepareEQFilters(spec);
    
    // Filters
    prepareFilters(spec);
    
    // Oversampling
    oversamplingProcessor.initProcessing(spec.maximumBlockSize);
    oversamplingProcessor.reset();
    oversamplingToggle = *treeState.getRawParameterValue(menuId);
}

void LV50AAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LV50AAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LV50AAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Audio block for dsp modules
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    juce::dsp::AudioBlock<float> upSampledBlock;
    
    /*==========================================================================*/
    
    // Toggle betweem oversampled and not
    if (oversamplingToggle)
    {
        // Oversample up
        setAllSampleRates(lastSampleRate);
        upSampledBlock = oversamplingProcessor.processSamplesUp(audioBlock);
        
        // Filters
        mLPFilter.process(upSampledBlock, buffer.getNumChannels());
        mHPFilter.process(upSampledBlock, buffer.getNumChannels());
        
        // EQ Filters
        mLSFilter.process(upSampledBlock, buffer.getNumChannels());
        mBPFilter1.process(upSampledBlock, buffer.getNumChannels());
        mBPFilter2.process(upSampledBlock, buffer.getNumChannels());
        mHSFilter.process(upSampledBlock, buffer.getNumChannels());
        
        if (mRawDrive != 0.0f)
        {
            // Main process loop
            callProcessorCore(upSampledBlock, buffer.getNumChannels());
        }
        
        oversamplingProcessor.processSamplesDown(audioBlock);
    }
    
    else
    {
        setAllSampleRates(projectSampleRate);
        
        // Filters
        mLPFilter.process(audioBlock, buffer.getNumChannels());
        mHPFilter.process(audioBlock, buffer.getNumChannels());
        
        // EQ Filters
        mLSFilter.process(audioBlock, buffer.getNumChannels());
        mBPFilter1.process(audioBlock, buffer.getNumChannels());
        mBPFilter2.process(audioBlock, buffer.getNumChannels());
        mHSFilter.process(audioBlock, buffer.getNumChannels());
        
        if (mRawDrive != 0.0f)
        {
            // Main process loop
            callProcessorCore(audioBlock, buffer.getNumChannels());
        }
    }
    
    // Trim gain and smoothing
    if (currentTrimGain == lastTrimGain)
    {
        buffer.applyGain(0, buffer.getNumSamples(), currentTrimGain);

    }
    
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), lastTrimGain, currentTrimGain);
        lastTrimGain = currentTrimGain;
    }
    
    // Phase
    buffer.applyGain(currentPhase);
}

juce::AudioProcessorValueTreeState::ParameterLayout LV50AAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Make sure to update the number of reservations after adding params
    params.reserve(15);
    
    auto pDrive = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0f, 12.0f, 0.0f);
    auto pTrim = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -24.0f, 24.0f, 0.0f);
    auto pPhase = std::make_unique<juce::AudioParameterInt>(phaseId, phaseName, 0, 1, 0);
    auto pLowShelf = std::make_unique<juce::AudioParameterInt>(lowShelfBId, lowShelfBName, 0, 1, 1);
    auto pHighShelf = std::make_unique<juce::AudioParameterInt>(highShelfBId, highShelfBName, 0, 1, 1);
    auto pFilters = std::make_unique<juce::AudioParameterBool>(filtersBId, filtersBName, false);
    
    auto pGain1 = std::make_unique<juce::AudioParameterInt>(gain1Id, gain1Name, -12, 12, 0);
    auto pGain2 = std::make_unique<juce::AudioParameterInt>(gain2Id, gain2Name, -12, 12, 0);
    auto pGain3 = std::make_unique<juce::AudioParameterInt>(gain3Id, gain3Name, -12, 12, 0);
    auto pGain4 = std::make_unique<juce::AudioParameterInt>(gain4Id, gain4Name, -12, 12, 0);

    auto pFreq1 = std::make_unique<juce::AudioParameterInt>(freq1Id, freq1Name, 30, 390, 210);
    auto pFreq2 = std::make_unique<juce::AudioParameterInt>(freq2Id, freq2Name, 200, 2000, 1000);
    auto pFreq3 = std::make_unique<juce::AudioParameterInt>(freq3Id, freq3Name, 200, 2000, 1000);
    auto pFreq4 = std::make_unique<juce::AudioParameterInt>(freq4Id, freq4Name, 1000, 20000, 7000);

    auto pMenu = std::make_unique<juce::AudioParameterInt>(menuId, menuName, 0, 1, 0);

    params.push_back(std::move(pDrive));
    params.push_back(std::move(pTrim));
    params.push_back(std::move(pPhase));
    params.push_back(std::move(pLowShelf));
    params.push_back(std::move(pHighShelf));
    params.push_back(std::move(pFilters));
    
    params.push_back(std::move(pGain1));
    params.push_back(std::move(pGain2));
    params.push_back(std::move(pGain3));
    params.push_back(std::move(pGain4));
    
    params.push_back(std::move(pFreq1));
    params.push_back(std::move(pFreq2));
    params.push_back(std::move(pFreq3));
    params.push_back(std::move(pFreq4));
    
    params.push_back(std::move(pMenu));

    return { params.begin(), params.end() };
}

void LV50AAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue){
    
    // Update params here
    if (parameterID == driveSliderId){
        
        mRawDrive = newValue;
    }

    else if (parameterID == trimSliderId) {
        
        currentTrimGain = pow(10, newValue / 20);
        
    } else  if (parameterID == phaseId) {
        
        setPhase(newValue);
        
    } else if (parameterID == gain1Id){
        
        if (newValue == 0) {
            
            mLSFilter.setParameter(LV_SVFilter::ParameterId::kBypass, true);
            
        } else {
            
            mLSFilter.setParameter(LV_SVFilter::ParameterId::kBypass, false);
            mLSFilter.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
            mLSFilter.setParameter(LV_SVFilter::ParameterId::kQ, proportionalShelfFilterQ(newValue, true));
        }
        
    } else if (parameterID == gain2Id){
        
        if (newValue == 0) {
            
            mBPFilter1.setParameter(LV_SVFilter::ParameterId::kBypass, true);
            
        } else {
            
            mBPFilter1.setParameter(LV_SVFilter::ParameterId::kBypass, false);
            mBPFilter1.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
            mBPFilter1.setParameter(LV_SVFilter::ParameterId::kQ, proportionalPeakFilterQ(newValue));
        }

    } else if (parameterID == gain3Id){
        
        if (newValue == 0) {
            
            mBPFilter2.setParameter(LV_SVFilter::ParameterId::kBypass, true);
            
        } else {
            
            mBPFilter2.setParameter(LV_SVFilter::ParameterId::kBypass, false);
            mBPFilter2.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
            mBPFilter2.setParameter(LV_SVFilter::ParameterId::kQ, proportionalPeakFilterQ(newValue));
        }
        
    } else if (parameterID == gain4Id){
        
        if (newValue == 0) {
            
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kBypass, true);
            
        } else {
            
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kBypass, false);
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kQ, proportionalShelfFilterQ(newValue, false));
        }
        
    } else if (parameterID == freq1Id){
        
        mLSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
        
    } else if (parameterID == freq2Id){
        
        mBPFilter1.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
        
    } else if (parameterID == freq3Id){
        
        mBPFilter2.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
        
    } else if (parameterID == freq4Id){
        
        if (*treeState.getRawParameterValue(highShelfBId) == 1){
            
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(newValue) * 0.6f);

        } else {
            
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(newValue));
        }
        
    } else if (parameterID == lowShelfBId) {
        
        if (newValue == 0) {
            
            mLSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
            
        } else {
            
            mLSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kLowShelf);
        }
        
    } else if (parameterID == highShelfBId){
        
        if (newValue == 0) {
            
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kBandShelf);
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(*treeState.getRawParameterValue(freq4Id)));
            
        } else {
            
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kType, LV_SVFilter::kHighShelf);
            mHSFilter.setParameter(LV_SVFilter::ParameterId::kCutoff, float(*treeState.getRawParameterValue(freq4Id)) * 0.6f);
        }
        
    } else if (parameterID == menuId) {
        
        oversamplingToggle = newValue;
        
    } else if (parameterID == filtersBId){
        
        mLPFilter.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
        mHPFilter.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
    }
}
//==============================================================================
bool LV50AAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LV50AAudioProcessor::createEditor()
{
    return new LV50AAudioProcessorEditor (*this);
}

//==============================================================================
void LV50AAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void LV50AAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        treeState.state = tree;
    }
    
    oversamplingToggle = *treeState.getRawParameterValue(menuId);
    
    windowWidth = variableTree.getProperty("width");
    windowHeight = variableTree.getProperty("height");
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LV50AAudioProcessor();
}

void LV50AAudioProcessor::setPhase(int phase)
{
    if (phase == 0){
        
        currentPhase = 1;
        
    } else {
        
        currentPhase = -1;
    }
}
