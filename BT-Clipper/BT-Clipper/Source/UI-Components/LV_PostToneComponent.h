/*
  ==============================================================================

    LV_PostToneComponent.h
    Created: 14 Jan 2022 7:11:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../Widgets/WidgetIncludes.h"
#include "../Parameters/ParamDefs.h"
//==============================================================================
/*
*/
class LV_PostToneComponent  : public juce::Component
{
public:
    LV_PostToneComponent(BTClipperAudioProcessor& p);
    ~LV_PostToneComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setWidthAndHeight(float w, float h);

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    juce::LV_Toggle moduleBypass;
    juce::LV_GroupComponent preGroup;
    
    juce::LV_Dial freqDial {" Hz", 250.0, 2500.0, 1.0, 1000.0};
    juce::LV_Dial gainDial {" dB", -12.0, 12.0, 0.1, 0.0};
    juce::LV_Dial qDial {" %", 0.01, 0.99, 0.1, 0.3};
    
    juce::LV_Label freqDialLabel;
    juce::LV_Label gainDialLabel;
    juce::LV_Label qDialLabel;
    
    std::unique_ptr<SliderAttachment> freqDialAttach;
    std::unique_ptr<SliderAttachment> gainDialAttach;
    std::unique_ptr<SliderAttachment> qDialAttach;
    std::unique_ptr<ButtonAttachment> powerAttach;
    
    juce::LV_GroupComponent postGroup;
    
    /** Vars ==================================================================*/
    float width {0.0f};
    float height {0.0f};
    
    BTClipperAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_PostToneComponent)
};
