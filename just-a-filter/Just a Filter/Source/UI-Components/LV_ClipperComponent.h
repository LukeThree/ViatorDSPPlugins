/*
  ==============================================================================

    LV_ClipperComponent.h
    Created: 14 Jan 2022 5:33:49pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../Widgets/WidgetIncludes.h"
//==============================================================================
/*
*/
class LV_ClipperComponent  : public juce::Component
{
public:
    LV_ClipperComponent(JustaFilterAudioProcessor& p);
    ~LV_ClipperComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setWidthAndHeight(float w, float h);

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    juce::LV_Toggle moduleBypass;
    
    juce::LV_Fader driveFader {" dB", 0.0, 24.0, 0.01, 0.0};
    juce::LV_Label driveFaderLabel;
    
    std::unique_ptr<SliderAttachment> driveFaderAttach;
    std::unique_ptr<SliderAttachment> ceilingDialAttach;
    std::unique_ptr<SliderAttachment> trimDialAttach;
    std::unique_ptr<SliderAttachment> mixDialAttach;
    std::unique_ptr<ButtonAttachment> powerAttach;
    
    juce::LV_Dial ceilingDial {" dB", -60.0, 0.0, 0.1, 0.0};
    juce::LV_Dial mixDial {" dB", 0.0, 100.0, 1.0, 100.0};
    juce::LV_Dial trimDial {" dB", -96.0, 24.0, 0.1, 0.0};
    
    juce::LV_Label ceilingDialLabel;
    juce::LV_Label mixDialLabel;
    juce::LV_Label trimDialLabel;
    
    juce::LV_GroupComponent clipGroup;
    juce::LV_GroupComponent scopeGroup;
    
    juce::Image osc;
    
    /** Vars ==================================================================*/
    float width {0.0f};
    float height {0.0f};
    juce::Colour scopeColor {juce::Colour::fromRGB(135, 177, 145)};
    
    JustaFilterAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_ClipperComponent)
};
