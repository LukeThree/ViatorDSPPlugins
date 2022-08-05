/*
  ==============================================================================

    ParametricEQComponent.h
    Created: 12 Apr 2022 7:10:38pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class ParametricEQComponent  : public juce::Component
{
public:
    ParametricEQComponent(MultiQAudioProcessor& p);
    ~ParametricEQComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void reset(bool shouldReset);

private:
    
    MultiQAudioProcessor& audioProcessor;
    
    /** Dials and Labels*/
    viator_gui::Dial band1GainDial {" dB", "Low Gain", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band1Label;

    viator_gui::Dial band2GainDial {" dB", "Mid Gain", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band2Label;

    viator_gui::Dial band3GainDial {" dB", "Mid Gain", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band3Label;

    viator_gui::Dial band4GainDial {" dB", "High Gain", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band4Label;
    
    viator_gui::Dial band1FreqDial {" Hz", "Low Freq", 30.0, 400.0, 1.0, 0.0};
    viator_gui::Label band1FreqLabel;

    viator_gui::Dial band2FreqDial {" Hz", "Mid Freq", 200.0, 2000.0, 1.0, 0.0};
    viator_gui::Label band2FreqLabel;

    viator_gui::Dial band3FreqDial {" Hz", "Mid Freq", 200.0, 2000.0, 1.0, 0.0};
    viator_gui::Label band3FreqLabel;

    viator_gui::Dial band4FreqDial {" Hz", "High Freq", 1000.0, 20000.0, 1.0, 0.0};
    viator_gui::Label band4FreqLabel;
    
    /** Attachments*/
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> band1GainDialAttach;
    std::unique_ptr<SliderAttachment> band2GainDialAttach;
    std::unique_ptr<SliderAttachment> band3GainDialAttach;
    std::unique_ptr<SliderAttachment> band4GainDialAttach;
    
    std::unique_ptr<SliderAttachment> band1FreqDialAttach;
    std::unique_ptr<SliderAttachment> band2FreqDialAttach;
    std::unique_ptr<SliderAttachment> band3FreqDialAttach;
    std::unique_ptr<SliderAttachment> band4FreqDialAttach;
    
    /** Container for Dials */
    std::vector<viator_gui::Dial*> dials =
    {
        &band1GainDial, &band2GainDial, &band3GainDial, &band4GainDial, &band1FreqDial, &band2FreqDial, &band3FreqDial, &band4FreqDial
    };
    
    /** Container for Labels */
    std::vector<viator_gui::Label*> labels =
    {
        &band1Label, &band2Label, &band3Label, &band4Label, &band1FreqLabel, &band2FreqLabel, &band3FreqLabel, &band4FreqLabel
    };
    
    /** Container for Text */
    std::vector<juce::String> labelTexts =
    {
        "Low Gain", "Mid Gain", "Mid Gain", "High Gain", "Low Freq", "Mid Freq", "Mid Freq", "High Freq"
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametricEQComponent)
};
