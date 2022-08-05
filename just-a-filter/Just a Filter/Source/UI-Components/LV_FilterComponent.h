/*
  ==============================================================================

    LV_FilterComponent.h
    Created: 3 Feb 2022 9:02:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class LV_FilterComponent  : public juce::Component
{
public:
    LV_FilterComponent(JustaFilterAudioProcessor& p);
    ~LV_FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    JustaFilterAudioProcessor& audioProcessor;
    
    viator_gui::Border filterBorder;
    viator_gui::Dial cutoffDial {" Hz", "Cutoff", 20.0, 20000.0, 1.0, 1000.0};
    viator_gui::Dial qDial {" Q", "Q", 0.05, 0.95, 0.01, 0.33};
    viator_gui::Dial gainDial {" dB", "Gain", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Toggle phaseToggle;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<SliderAttachment> cutoffDialAttach;
    std::unique_ptr<SliderAttachment> qDialAttach;
    std::unique_ptr<SliderAttachment> gainDialAttach;
    std::unique_ptr<ButtonAttachment> phaseAttach;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_FilterComponent)
};
