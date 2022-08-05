/*
  ==============================================================================

    TubeEQComponent.h
    Created: 12 Apr 2022 7:10:54pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class TubeEQComponent  : public juce::Component
{
public:
    TubeEQComponent(MultiQAudioProcessor& p);
    ~TubeEQComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void reset(bool shouldReset);

private:
    
    MultiQAudioProcessor& audioProcessor;
    
    /** Dials and Labels*/
    viator_gui::Dial lowBoostDial {" dB", "Low Boost", 0.0, 10.0, 0.1, 0.0};
    viator_gui::Label lowBoostLabel;
    
    viator_gui::Dial lowCutDial {" dB", "Low Cut", 0.0, 10.0, 0.1, 0.0};
    viator_gui::Label lowCutLabel;
    
    viator_gui::Dial lowFreqDial {" Hz", "Low Freq", 30.0, 300.0, 1.0, 30.0};
    viator_gui::Label lowFreqLabel;
    
    viator_gui::Dial bandwidthDial {" BW", "Band Width", 0.0, 10.0, 0.1, 0.0};
    viator_gui::Label bandwidthLabel;
    
    viator_gui::Dial highBoostDial {" dB", "High Boost", 0.0, 10.0, 0.1, 0.0};
    viator_gui::Label highBoostLabel;
    
    viator_gui::Dial highCutDial {" dB", "High Cut", 0.0, 10.0, 0.1, 0.0};
    viator_gui::Label highCutLabel;
    
    viator_gui::Dial highFreqDial {" Hz", "High Freq", 1000.0, 16000.0, 1.0, 0.0};
    viator_gui::Label highFreqLabel;
    
    /** Attachments*/
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> lowBoostAttach;
    std::unique_ptr<SliderAttachment> lowCutAttach;
    std::unique_ptr<SliderAttachment> lowFreqAttach;
    std::unique_ptr<SliderAttachment> bandwidthAttach;
    std::unique_ptr<SliderAttachment> highBoostAttach;
    std::unique_ptr<SliderAttachment> highCutAttach;
    std::unique_ptr<SliderAttachment> highFreqAttach;
    
    /** Container for Dials */
    std::vector<viator_gui::Dial*> dials =
    {
        &lowBoostDial, &lowCutDial, &lowFreqDial, &bandwidthDial, &highBoostDial, &highCutDial, &highFreqDial
    };
    
    /** Container for Labels */
    std::vector<viator_gui::Label*> labels =
    {
        &lowBoostLabel, &lowCutLabel, &lowFreqLabel, &bandwidthLabel, &highBoostLabel, &highCutLabel, &highFreqLabel
    };
    
    /** Container for Text */
    std::vector<juce::String> labelTexts =
    {
        "Low Boost", "Low Cut", "Low Freq", "Band Width", "High Boost", "High Cut", "High Freq"
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TubeEQComponent)
};
