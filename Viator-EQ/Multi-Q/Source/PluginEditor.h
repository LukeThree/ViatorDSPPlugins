/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./UI-Components/LV_Window.h"
#include "./UI-Components/GraphicEQComponent.h"
#include "./UI-Components/ParametricEQComponent.h"
#include "TubeEQComponent.h"

//==============================================================================
/**
*/
class MultiQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultiQAudioProcessorEditor (MultiQAudioProcessor&);
    ~MultiQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultiQAudioProcessor& audioProcessor;
    
    /** Boiler plate UI stuff */
    void uiConstructor();
    void initWindow();
    void uiPaint(juce::Graphics &g);
    void uiResized();
    void saveWindowSize();
    bool constructorFinished = false;
    
    /** Window */
    LV_Window windowComponent;
    
    /** Graphic EQ*/
    GraphicEQComponent graphicEQComponent;
    
    /** Parametric EQ*/
    ParametricEQComponent parametricEQComponent;
    
    /** Tube EQ */
    TubeEQComponent tubeEQComponent;
    
    /** Output Section*/
    viator_gui::Fader hpFader {" Hz", "HP", 20.0, 1000.0, 1.0, 20.0};
    viator_gui::Label hpFaderLabel;
    
    viator_gui::Fader lpFader {" Hz", "LP", 1000.0, 20000.0, 1.0, 20000.0};
    viator_gui::Label lpFaderLabel;
    
    viator_gui::Fader driveFader {" dB", "Drive", 0.0, 12.0, 0.01, 0.0};
    viator_gui::Label driveFaderLabel;
    
    viator_gui::Fader trimFader {" dB", "Trim", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label trimFaderLabel;
    
    viator_gui::ToggleButton graphicEQButton;
    viator_gui::ToggleButton paraEQButton;
    viator_gui::ToggleButton tubeEQButton;
    viator_gui::Toggle phaseToggle {""};
    viator_gui::PushButton resetButton;
    
    /** Attachments */
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> hpFaderAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lpFaderAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveFaderAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> trimFaderAttach;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> graphicButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> parametricButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> tubeButtonAttach;
    
    /** Container for Faders */
    std::vector<viator_gui::Fader*> faders =
    {
        &hpFader, &lpFader, &driveFader, &trimFader
    };
    
    /** Container for Labels */
    std::vector<viator_gui::Label*> labels =
    {
        &hpFaderLabel, &lpFaderLabel, &driveFaderLabel, &trimFaderLabel
    };
    
    /** Container for Text */
    std::vector<juce::String> labelTexts =
    {
        "HP", "LP", "Drive", "Trim"
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiQAudioProcessorEditor)
};
