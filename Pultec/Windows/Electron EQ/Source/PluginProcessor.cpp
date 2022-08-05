/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ElectronEQAudioProcessor::ElectronEQAudioProcessor()
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
osModule (2, 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR, true, false)

#endif
{
    treeState.addParameterListener (driveSliderId, this);
    treeState.addParameterListener (osToggleId, this);
    treeState.addParameterListener (lpfSliderId, this);
    treeState.addParameterListener (trimSliderId, this);
    treeState.addParameterListener (stereoMenuId, this);
    treeState.addParameterListener (highBoostDialId, this);
    treeState.addParameterListener (highFreqDialId, this);
    treeState.addParameterListener (highAttenDialId, this);
    treeState.addParameterListener (lowBoostDialId, this);
    treeState.addParameterListener (lowFreqDialId, this);
    treeState.addParameterListener (lowAttenDialId, this);
    treeState.addParameterListener (bandwidthDialId, this);
    treeState.addParameterListener (eqToggleId, this);
    
    variableTree = {
                
                "ElectronVariables", {},
                {
                  { "Group", {{ "name", "ElectronVars" }},
                    {
                      { "Parameter", {{ "id", "width" }, { "value", 0 }}},
                        { "Parameter", {{ "id", "height" }, { "value", 0 }}},
                    }
                  }
                }
              };
}

ElectronEQAudioProcessor::~ElectronEQAudioProcessor()
{
    treeState.removeParameterListener (driveSliderId, this);
    treeState.removeParameterListener (osToggleId, this);
    treeState.removeParameterListener (lpfSliderId, this);
    treeState.removeParameterListener (trimSliderId, this);
    treeState.removeParameterListener (stereoMenuId, this);
    treeState.removeParameterListener (highBoostDialId, this);
    treeState.removeParameterListener (highFreqDialId, this);
    treeState.removeParameterListener (highAttenDialId, this);
    treeState.removeParameterListener (lowBoostDialId, this);
    treeState.removeParameterListener (lowFreqDialId, this);
    treeState.removeParameterListener (lowAttenDialId, this);
    treeState.removeParameterListener (bandwidthDialId, this);
    treeState.removeParameterListener (eqToggleId, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout ElectronEQAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Make sure to update the number of reservations after adding params
    params.reserve(13);
    
    auto pDrive = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0, 16.0, 0.0);
    auto pLPF = std::make_unique<juce::AudioParameterInt>(lpfSliderId, lpfSliderName, 3000, 20000, 20000);
    auto pTrim = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -48.0, 48.0, 0.0);
    auto pStereoMenu = std::make_unique<juce::AudioParameterInt>(stereoMenuId, stereoMenuName, 0, 4, 0);
    auto pHighBoost = std::make_unique<juce::AudioParameterFloat>(highBoostDialId, highBoostDialName, 0.0, 10.0, 0.0);
    auto pHighAtten = std::make_unique<juce::AudioParameterFloat>(highAttenDialId, highAttenDialName, 0.0, 10.0, 0.0);
    auto pHighFreq = std::make_unique<juce::AudioParameterInt>(highFreqDialId, highFreqDialName, 1000, 16000, 6000);
    auto pLowBoost = std::make_unique<juce::AudioParameterFloat>(lowBoostDialId, lowBoostDialName, 0.0, 10.0, 0.0);
    auto pLowAtten = std::make_unique<juce::AudioParameterFloat>(lowAttenDialId, lowAttenDialName, 0.0, 10.0, 0.0);
    auto pLowFreq = std::make_unique<juce::AudioParameterInt>(lowFreqDialId, lowFreqDialName, 20, 300, 100);
    auto pBandwidth = std::make_unique<juce::AudioParameterFloat>(bandwidthDialId, bandwidthDialName, 0.0, 10.0, 5.0);
    auto pOsToggle = std::make_unique<juce::AudioParameterBool>(osToggleId, osToggleName, false);
    auto pEqToggle = std::make_unique<juce::AudioParameterBool>(eqToggleId, eqToggleName, true);

    params.push_back(std::move(pDrive));
    params.push_back(std::move(pLPF));
    params.push_back(std::move(pTrim));
    params.push_back(std::move(pStereoMenu));
    params.push_back(std::move(pHighBoost));
    params.push_back(std::move(pHighAtten));
    params.push_back(std::move(pHighFreq));
    params.push_back(std::move(pLowBoost));
    params.push_back(std::move(pLowAtten));
    params.push_back(std::move(pLowFreq));
    params.push_back(std::move(pBandwidth));
    params.push_back(std::move(pOsToggle));
    params.push_back(std::move(pEqToggle));

    return { params.begin(), params.end() };
}

void ElectronEQAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == driveSliderId)
    {
        tubeModule.setParameter(LV_TubeModel::ParameterId::kPreamp, newValue);
    }
    
    if (parameterID == osToggleId)
    {
        osModuleToggle = newValue;
    }
    
    if (parameterID == lpfSliderId)
    {
        lpfModule.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
    }
    
    if (parameterID == trimSliderId)
    {
        trimModule.setGainDecibels(newValue);
    }
    
    if (parameterID == stereoMenuId)
    {
        stereoMenuChoice = newValue;
    }
    
    if (parameterID == highBoostDialId)
    {
        highBoostModule.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
    }
    
    if (parameterID == highAttenDialId)
    {
        highCutModlue.setParameter(LV_SVFilter::ParameterId::kGain, newValue * -2.0);

    }
    
    if (parameterID == highFreqDialId)
    {
        highBoostModule.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
        highCutModlue.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue / 4.0);
    }
    
    if (parameterID == lowBoostDialId)
    {
        lowBoostModule.setParameter(LV_SVFilter::ParameterId::kGain, newValue);
    }
    
    if (parameterID == lowFreqDialId)
    {
        lowBoostModule.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue);
        lowCutModule.setParameter(LV_SVFilter::ParameterId::kCutoff, newValue * 2.0);
    }
    
    if (parameterID == lowAttenDialId)
    {
        lowCutModule.setParameter(LV_SVFilter::ParameterId::kGain, newValue * -1.0);
    }
    
    if (parameterID == bandwidthDialId)
    {
        auto scalar = juce::jmap(newValue, 0.0f, 10.0f, 0.8f, 0.25f);
        
        highBoostModule.setParameter(LV_SVFilter::ParameterId::kQ, scalar);
    }
    
    if (parameterID == eqToggleId)
    {
        highBoostModule.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
        highCutModlue.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
        lowBoostModule.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
        lowCutModule.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
        lpfModule.setParameter(LV_SVFilter::ParameterId::kBypass, !newValue);
    }
}

//==============================================================================
const juce::String ElectronEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ElectronEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ElectronEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ElectronEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ElectronEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ElectronEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ElectronEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ElectronEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ElectronEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void ElectronEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ElectronEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate * osModule.getOversamplingFactor();
    spec.numChannels = getTotalNumOutputChannels();
    
    // We need both samplerates to switch between oversampling states
    lastSampleRate = spec.sampleRate;
    projectSampleRate = sampleRate;
    
    // Prepare dsp
    prepareFilters(spec);
    prepareTube(spec);
    prepareTrim(spec);
    osModule.initProcessing(samplesPerBlock);
    stereoMenuChoice = *treeState.getRawParameterValue(stereoMenuId);
}

void ElectronEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ElectronEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ElectronEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Audio block for dsp modules
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    juce::dsp::AudioBlock<float> upSampledBlock;
    
    // Toggle betweem oversampled and not
    if (osModuleToggle)
    {
        // Oversample up
        //setAllSampleRates(lastSampleRate);
        lowCutModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, lastSampleRate);
        lowBoostModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, lastSampleRate);
        highCutModlue.setParameter(LV_SVFilter::ParameterId::kSampleRate, lastSampleRate);
        highBoostModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, lastSampleRate);
        lpfModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, lastSampleRate);
        upSampledBlock = osModule.processSamplesUp(audioBlock);
    }
        
    else
    {
        // Normal
        //setAllSampleRates(projectSampleRate);
        lowCutModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, projectSampleRate);
        lowBoostModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, projectSampleRate);
        highCutModlue.setParameter(LV_SVFilter::ParameterId::kSampleRate, projectSampleRate);
        highBoostModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, projectSampleRate);
        lpfModule.setParameter(LV_SVFilter::ParameterId::kSampleRate, projectSampleRate);
        upSampledBlock = audioBlock;
    }
    
    /**Sample-based processing loop*/
    for (auto channel {0}; channel < upSampledBlock.getNumChannels(); channel++)
    {
        
        auto *channelData = upSampledBlock.getChannelPointer(channel);
        
        auto *leftChannelData = upSampledBlock.getChannelPointer(0);
        
        // if 2 channels, 1, else 0
        auto *rightChannelData = upSampledBlock.getChannelPointer(totalNumInputChannels > 1);
        
        for (auto sample {0}; sample < upSampledBlock.getNumSamples(); sample++)
        {
            auto stereo_x = channelData[sample];
            auto mid_x = 0.5 * (leftChannelData[sample] + rightChannelData[sample]);
            auto side_x = 0.5 * (leftChannelData[sample] - rightChannelData[sample]);
            auto left_x = leftChannelData[sample];
            auto right_x = rightChannelData[sample];
            
            switch (stereoMenuChoice)
            {
                case 0:
                {
                    output = lowCutModule.processSample(stereo_x, channel);
                    output = lowBoostModule.processSample(output, channel);
                    output = highCutModlue.processSample(output, channel);
                    output = highBoostModule.processSample(output, channel);
                    output = lpfModule.processSample(output, channel);
                    output = tubeModule.processSample(output, channel);

                    channelData[sample] = output;
                    
                    break;
                }
                    
                case 1:
                {
                    output = lowCutModule.processSample(side_x, channel);
                    output = lowBoostModule.processSample(output, channel);
                    output = highCutModlue.processSample(output, channel);
                    output = highBoostModule.processSample(output, channel);
                    output = lpfModule.processSample(output, channel);
                    output = tubeModule.processSample(output, channel);
                    
                    auto newL = mid_x + output;
                    auto newR = mid_x - output;
                    
                    leftChannelData[sample] = newL;
                    rightChannelData[sample] = newR;
                    
                    break;
                }
                    
                case 2:
                {
                    output = lowCutModule.processSample(mid_x, channel);
                    output = lowBoostModule.processSample(output, channel);
                    output = highCutModlue.processSample(output, channel);
                    output = highBoostModule.processSample(output, channel);
                    output = lpfModule.processSample(output, channel);
                    output = tubeModule.processSample(output, channel);

                    auto newL = output + side_x;
                    auto newR = output - side_x;
                    
                    leftChannelData[sample] = newL;
                    rightChannelData[sample] = newR;
                    
                    break;
                }
                    
                case 3:
                {
                    output = lowCutModule.processSample(left_x, channel);
                    output = lowBoostModule.processSample(output, channel);
                    output = highCutModlue.processSample(output, channel);
                    output = highBoostModule.processSample(output, channel);
                    output = lpfModule.processSample(output, channel);
                    output = tubeModule.processSample(output, channel);
                    
                    leftChannelData[sample] = output;
                    rightChannelData[sample] = right_x;
                    
                    break;
                }
                    
                case 4:
                {
                    output = lowCutModule.processSample(right_x, channel);
                    output = lowBoostModule.processSample(output, channel);
                    output = highCutModlue.processSample(output, channel);
                    output = highBoostModule.processSample(output, channel);
                    output = lpfModule.processSample(output, channel);
                    output = tubeModule.processSample(output, channel);
                    
                    leftChannelData[sample] = left_x;
                    rightChannelData[sample] = output;
                    
                    break;
                }
            }
        }
    }
    
    // Sample down if oversampling is on
    if (osModuleToggle)
    {
        osModule.processSamplesDown(audioBlock);
    }
    
    trimModule.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
}

//==============================================================================
bool ElectronEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ElectronEQAudioProcessor::createEditor()
{
    return new ElectronEQAudioProcessorEditor (*this);
}

//==============================================================================
void ElectronEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void ElectronEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
    variableTree = tree.getChildWithName("ElectronVariables");

    if (tree.isValid())
    {
        treeState.state = tree;
    }

    recallFilters();
    recallTube();
    recallTrim();
    recallStereoChoice();
    recallEqBypass();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ElectronEQAudioProcessor();
}
