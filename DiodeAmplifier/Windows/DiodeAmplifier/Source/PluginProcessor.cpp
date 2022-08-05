/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DiodeAmplifierAudioProcessor::DiodeAmplifierAudioProcessor()
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
    treeState.addParameterListener (inputGainSliderId, this);
    treeState.addParameterListener (driveSliderId, this);
    treeState.addParameterListener (lowSliderId, this);
    treeState.addParameterListener (midSliderId, this);
    treeState.addParameterListener (highSliderId, this);
    treeState.addParameterListener (outputGainSliderId, this);
    treeState.addParameterListener (brightId, this);
    treeState.addParameterListener (cabId, this);
    treeState.addParameterListener (menuId, this);
    
    variableTree = {
            
            "DiodeVariables", {},
            {
              { "Group", {{ "name", "DiodeVars" }},
                {
                  { "Parameter", {{ "id", "file" }, { "value", "/" }}},
                    { "Parameter", {{ "id", "root" }, { "value", "/" }}},
                    { "Parameter", {{ "id", "cabOffGain" }, { "value", -16.0 }}}
                }
              }
            }
          };
    
    variableTree.setProperty("cabOffGain", -16.0, nullptr);
    
    convolutionProcessor.loadImpulseResponse
    (BinaryData::metalOne_wav,
     BinaryData::metalOne_wavSize,
     juce::dsp::Convolution::Stereo::yes,
     juce::dsp::Convolution::Trim::yes, 0,
     juce::dsp::Convolution::Normalise::yes);
    
}

DiodeAmplifierAudioProcessor::~DiodeAmplifierAudioProcessor()
{
    treeState.removeParameterListener (inputGainSliderId, this);
    treeState.removeParameterListener (driveSliderId, this);
    treeState.removeParameterListener (lowSliderId, this);
    treeState.removeParameterListener (midSliderId, this);
    treeState.removeParameterListener (highSliderId, this);
    treeState.removeParameterListener (outputGainSliderId, this);
    treeState.removeParameterListener (brightId, this);
    treeState.removeParameterListener (cabId, this);
    treeState.removeParameterListener (menuId, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout DiodeAmplifierAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(8);
    
    auto inputGainParam = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -24.0f, 24.0f, 0.0f);
    auto driveParam = std::make_unique<juce::AudioParameterFloat>(driveSliderId, driveSliderName, 0.0f, 10.0f, 0.0f);
    auto lowParam = std::make_unique<juce::AudioParameterFloat>(lowSliderId, lowSliderName, -6.0f, 6.0f, 0.0f);
    auto midParam = std::make_unique<juce::AudioParameterFloat>(midSliderId, midSliderName, -6.0f, 6.0f, 0.0f);
    auto highParam = std::make_unique<juce::AudioParameterFloat>(highSliderId, highSliderName, -6.0f, 6.0f, 0.0f);
    auto outputGainParam = std::make_unique<juce::AudioParameterFloat>(outputGainSliderId, outputGainSliderName, -24.0f, 24.0f, 0.0f);
    auto brightParam = std::make_unique<juce::AudioParameterBool>(brightId, brightName, false);
    auto cabParam = std::make_unique<juce::AudioParameterBool>(cabId, cabName, true);
    auto pMenu = std::make_unique<juce::AudioParameterInt>(menuId, menuName, 0, 1, 0);

    params.push_back(std::move(inputGainParam));
    params.push_back(std::move(driveParam));
    params.push_back(std::move(lowParam));
    params.push_back(std::move(midParam));
    params.push_back(std::move(highParam));
    params.push_back(std::move(outputGainParam));
    params.push_back(std::move(brightParam));
    params.push_back(std::move(cabParam));
    params.push_back(std::move(pMenu));

    return { params.begin(), params.end() };
}

void DiodeAmplifierAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == inputGainSliderId)
        {
            inputGainProcessor.setGainDecibels(newValue);
        }
    
    else if (parameterID == lowSliderId)
        {
            updateLowFilter(newValue);
        }
    
    else if (parameterID == midSliderId)
        {
            updateMidFilter(newValue);
        }
    
    else if (parameterID == highSliderId)
        {
            updateHighFilter(newValue);
        }
    
    else if (parameterID == outputGainSliderId)
        {
            outputGainProcessor.setGainDecibels(newValue);
            
            if (!convolutionToggle) variableTree.setProperty("cabOffGain", newValue, nullptr);
        }
    
    else if (parameterID == driveSliderId)
        {
            driveScaled = pow(10.0f, newValue * 0.25f);
        }
    
    else if (parameterID == brightId)
        {
            *highNotchFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 4000.0f, 1.0f, pow(10.0f, -12.0f / 20.0f) * (newValue + 1));

        }
    else if (parameterID == cabId)
        {
            convolutionToggle = newValue;
            
            if (newValue == 0)
            {
                treeState.getParameterAsValue(outputGainSliderId) = variableTree.getProperty("cabOffGain");
                DBG(variableTree.getProperty("cabOffGain").toString());
            }
            
            else
            {
                treeState.getParameterAsValue(outputGainSliderId) = 0.0;
            }
        }
    else if (parameterID == menuId)
        {
            oversamplingToggle = newValue;
        }
}

//==============================================================================
const juce::String DiodeAmplifierAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DiodeAmplifierAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DiodeAmplifierAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DiodeAmplifierAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DiodeAmplifierAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DiodeAmplifierAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DiodeAmplifierAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DiodeAmplifierAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DiodeAmplifierAudioProcessor::getProgramName (int index)
{
    return {};
}

void DiodeAmplifierAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DiodeAmplifierAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
    
    highPassFilter.prepare(spec);
    highPassFilter.reset();
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(projectSampleRate, 200);

    preClipFilter.prepare(spec);
    preClipFilter.reset();
    *preClipFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(projectSampleRate, 1420, 0.5, 6.0);
    
    lowFilter.prepare(spec);
    lowFilter.reset();
    *lowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(projectSampleRate, 200, 1.3, pow(10, *treeState.getRawParameterValue(lowSliderId) * 0.05));

    midFilter.prepare(spec);
    midFilter.reset();
    *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(projectSampleRate, 815, 0.3, pow(10, *treeState.getRawParameterValue(midSliderId) * 0.05));

    highFilter.prepare(spec);
    highFilter.reset();
    *highFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(projectSampleRate, 6000, 0.2, pow(10, *treeState.getRawParameterValue(highSliderId) * 0.05));

    highNotchFilter.prepare(spec);
    highNotchFilter.reset();
    *highNotchFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(projectSampleRate, 4000.0f, 1.0f, pow(10.0f, -12.0f / 20.0f) * (*treeState.getRawParameterValue(brightId) + 1));
    
    
    inputGainProcessor.prepare(spec);
    inputGainProcessor.reset();
    inputGainProcessor.setGainDecibels(*treeState.getRawParameterValue(inputGainSliderId));
    
    outputGainProcessor.prepare(spec);
    outputGainProcessor.reset();
    outputGainProcessor.setGainDecibels(*treeState.getRawParameterValue(outputGainSliderId));

    convolutionProcessor.prepare(spec);
    
    convolutionToggle = *treeState.getRawParameterValue(cabId);
        
    driveScaled = pow(10.0f, *treeState.getRawParameterValue(driveSliderId) * 0.25f);
}

void DiodeAmplifierAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DiodeAmplifierAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DiodeAmplifierAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock {buffer};

    inputGainProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    highPassFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    preClipFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
                auto* inputData = buffer.getReadPointer(channel);
                auto* outputData = buffer.getWritePointer(channel);

                for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

                    float diodeClippingAlgorithm = exp((0.1 * inputData[sample]) / (0.0253 * 1.68)) - 1;
                    
                    outputData[sample] = piDivisor * atan(diodeClippingAlgorithm * (driveScaled * 16));
                }
            }

    lowFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    midFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    highFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    highNotchFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    if (convolutionToggle) convolutionProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    outputGainProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void DiodeAmplifierAudioProcessor::setAllSampleRates(float value)
{
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(value, 200);
    *preClipFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(value, 1420, 0.5, 6.0);
    *lowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(value, 200, 1.3, pow(10, *treeState.getRawParameterValue(lowSliderId) * 0.05));
    *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(value, 815, 0.3, pow(10, *treeState.getRawParameterValue(midSliderId) * 0.05));
    *highFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(value, 6000, 0.2, pow(10, *treeState.getRawParameterValue(highSliderId) * 0.05));
    *highNotchFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(value, 4000.0f, 1.0f, pow(10.0f, -12.0f / 20.0f) * (*treeState.getRawParameterValue(brightId) + 1));

}

void DiodeAmplifierAudioProcessor::updateHighPassFilter(const float &freq){
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, 200);
}

void DiodeAmplifierAudioProcessor::updatePreClipFilter(const float &freq){
    *preClipFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 1420, 0.5, 6.0);
}

void DiodeAmplifierAudioProcessor::updateLowFilter(const float &gain){
    *lowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(lastSampleRate, 200, 1.3, pow(10, gain * 0.05));
}

void DiodeAmplifierAudioProcessor::updateMidFilter(const float &gain){
    *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 815, 0.3, pow(10, gain * 0.05));
}

void DiodeAmplifierAudioProcessor::updateHighFilter(const float &gain){
    *highFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, 6000, 0.2, pow(10, gain * 0.05));
}

//==============================================================================
bool DiodeAmplifierAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DiodeAmplifierAudioProcessor::createEditor()
{
    return new DiodeAmplifierAudioProcessorEditor (*this);
}

//==============================================================================
void DiodeAmplifierAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    treeState.state.appendChild(variableTree, nullptr);

    juce::MemoryOutputStream stream(destData, false);
            
    treeState.state.writeToStream (stream);
}

void DiodeAmplifierAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
        
    variableTree = tree.getChildWithName("DiodeVariables");
        
    if (tree.isValid())
    {
        treeState.state = tree;
    }
    
    savedFile = juce::File(variableTree.getProperty("file"));
    root = juce::File(variableTree.getProperty("root"));

    
    if (savedFile.existsAsFile())
    {
        convolutionProcessor.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
        DBG("Location exists as file");
    }
    
    else
    {
        convolutionProcessor.loadImpulseResponse
        (BinaryData::metalOne_wav,
            BinaryData::metalOne_wavSize,
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::yes, 0,
            juce::dsp::Convolution::Normalise::yes);
    }
    
    if (!convolutionToggle) treeState.getParameterAsValue(outputGainSliderId) = variableTree.getProperty("cabOffGain");
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DiodeAmplifierAudioProcessor();
}
