/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI-Components/LV_Window.h"
#include "UI-Components/LV_DialComponent.h"
//==============================================================================
/**
*/
class PhonemodelAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PhonemodelAudioProcessorEditor (PhonemodelAudioProcessor&);
    ~PhonemodelAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhonemodelAudioProcessor& audioProcessor;
    
    /** UI Methods ================================================================*/
    void uiConstructor();
    void uiPaint();
    void uiResized(float width, float height);
    
    /** Window ====================================================================*/
    LV_Window windowComponent;
    
    void initWindow();
    void saveWindowSize();
    bool constructorFinished {false};
    
    /** Images/Background =========================================================*/
    juce::Image pluginBackground;
    
    /** Dials =====================================================================*/
    LV_DialComponent bandwithDial;
    juce::Label bandwidthLabel {"bandwidthLabel", "Speaker Range"};
    
    LV_DialComponent centerDial;
    juce::Label centerLabel {"centerLabel", "Center Freq"};
    
    LV_DialComponent driveDial;
    juce::Label driveLabel {"driveLabel", "Dirt"};
    
    LV_DialComponent resoDial;
    juce::Label resoLabel {"resoLabel", "Resonance"};
    
    LV_DialComponent lowShelfDial;
    juce::Label lowShelfDialLabel {"lowGainLabel", "Low Gain"};
    
    LV_DialComponent highShelfDial;
    juce::Label highShelfDialLabel {"highGainLabel", "High Gain"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhonemodelAudioProcessorEditor)
};
