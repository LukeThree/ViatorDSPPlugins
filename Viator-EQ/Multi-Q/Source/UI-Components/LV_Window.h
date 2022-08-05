/*
  ==============================================================================

    LV_Window.h
    Created: 23 Oct 2021 12:53:38am
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "CPUComponent.h"

//==============================================================================
/*
*/
class LV_Window  : public juce::Component
, private juce::Timer
{
public:
    LV_Window(MultiQAudioProcessor& p);
    ~LV_Window() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override
    {
        cpu = audioProcessor.getCPULoad();
        cpuLabel.setText(std::to_string(static_cast<int>(cpu)) + "%", juce::dontSendNotification);
    }
    
private:
    
    MultiQAudioProcessor& audioProcessor;
    
    float cpu = 0;
    
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.patreon.com/ViatorDSP"};
    
    viator_gui::ToggleButton phaseToggle {true, "Phase"};
    viator_gui::Menu osMenu;
    viator_gui::Menu stereoMenu;
    
    viator_gui::Label cpuLabel;
    viator_gui::Label cpuHeader;
    viator_gui::LEDButton cpuButton;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> phaseButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> osAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> msAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> cpuButtonAttach;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_Window)
};
