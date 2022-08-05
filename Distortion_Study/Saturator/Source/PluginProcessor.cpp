/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessor::SaturatorAudioProcessor()
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
}

SaturatorAudioProcessor::~SaturatorAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SaturatorAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(3);
    
    auto driveParam = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0f, 24.0f, 0.0f);
    auto trimParam = std::make_unique<juce::AudioParameterFloat>(trimSliderId, trimSliderName, -36.0f, 36.0f, 0.0f);
    auto modelParam = std::make_unique<juce::AudioParameterInt>(modelId, modelName, 0, 6, 0);

    params.push_back(std::move(driveParam));
    params.push_back(std::move(trimParam));
    params.push_back(std::move(modelParam));
    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String SaturatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SaturatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SaturatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SaturatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SaturatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SaturatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SaturatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SaturatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SaturatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SaturatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SaturatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SaturatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaturatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SaturatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* outputData = buffer.getWritePointer (channel);
        auto* inputData = buffer.getReadPointer(channel);
        auto* rawDrive = treeState.getRawParameterValue(driveSliderId);
        auto* rawTrim = treeState.getRawParameterValue(trimSliderId);
        auto* rawModel = treeState.getRawParameterValue(modelId);
        int model = *rawModel;

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            
            switch (model) {
                case 0:
                    outputData[sample] = inputData[sample];
                    break;
                case 1:
                    outputData[sample] = softClip(inputData[sample], *rawDrive) * pow(10, *rawTrim * 0.05f);
                    break;
                case 2:
                    outputData[sample] = hardClip(inputData[sample], *rawDrive) * pow(10, *rawTrim * 0.05f);
                    break;
                case 4:
                    outputData[sample] = dcDistortion(inputData[sample], *rawDrive) * pow(10, *rawTrim * 0.05f);
                    break;
                case 5:
                    outputData[sample] = diode(inputData[sample], *rawDrive) * pow(10, *rawTrim * 0.05f);
                    break;
                case 6:
                    outputData[sample] = fullWaveRect(inputData[sample], *rawDrive) * pow(10, *rawTrim * 0.05f);
                    break;
                    
                default:
                    outputData[sample] = inputData[sample];
                    break;
            }
        }
    }
}

float SaturatorAudioProcessor::scaleRange(const float &input, const float &inputLow, const float &inputHigh, const float &outputLow, const float &outputHigh){
    return ((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow;
}

float SaturatorAudioProcessor::softClip(const float &input, const float &drive){
    
    //1.5f to account for drop in gain from the saturation initial state
    //pow(10, (-1 * drive) * 0.04f) to account for the increase in gain when the drive goes up
    
    return piDivisor * atan(pow(10, (drive * 4) * 0.05f) * input) * 1.5f * pow(10, (-1 * drive) * 0.04f);
}

float SaturatorAudioProcessor::hardClip(const float &input, const float &drive){
    
    float driveScaled = scaleRange(drive, 0.0f, 24.0f, 1.0f, 0.0001f);
    float output;
    
    if (input >= driveScaled){
        output = driveScaled;
    } else if (input <= (driveScaled * -1)){
        output = driveScaled * -1;
    } else {
        output = input;
    }
    
    return output * 1.5f * pow(10, drive * 0.04);
}

float SaturatorAudioProcessor::dcDistortion(const float &input, const float &drive){
    
    float dc = scaleRange(drive, 0.0f, 24.0, 0.75f, 1.0f);
    float x = (input * pow(10, drive * 0.05f)) + dc;
    float y;
    float output;
    
    if (abs(x) > 1) {
        x = sin(x);
    }
    
    y = x - 0.2f * pow(x, 5);
    
    output = (y - dc);
    
    return output;
}

float SaturatorAudioProcessor::diode(const float &input, const float &drive){
    
    float diodeClipper;
    
    diodeClipper = 0.105 * (exp(0.1 * input / (1.68 * 0.0253)) - 1);
    
    return diodeClipper;
}

float SaturatorAudioProcessor::fullWaveRect(const float &input, const float &drive){
    
    float inputScaled = input;
    float output;
    
    if (inputScaled >= 0.0f){
        output = input;
    } else {
        output = input * -1;
    }
    
    return output;
}

//==============================================================================
bool SaturatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SaturatorAudioProcessor::createEditor()
{
    return new SaturatorAudioProcessorEditor (*this);
}

//==============================================================================
void SaturatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
           treeState.state.writeToStream (stream);
}

void SaturatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
            if (tree.isValid()) {
                treeState.state = tree;
            }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaturatorAudioProcessor();
}
