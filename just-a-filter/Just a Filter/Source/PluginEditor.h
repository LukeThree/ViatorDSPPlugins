/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI-Components/LV_Window.h"
#include "UI-Components/LV_HeaderComponent.h"
#include "UI-Components/LV_FilterComponent.h"

//==============================================================================
/**
*/
class JustaFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JustaFilterAudioProcessorEditor (JustaFilterAudioProcessor&);
    ~JustaFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JustaFilterAudioProcessor& audioProcessor;
    
    /** UI Methods ================================================================*/
    void uiConstructor();
    void uiPaint(juce::Graphics& g);
    void uiResized(float width, float height);
        
    /** Window ====================================================================*/
    LV_Window windowComponent;
        
    void initWindow();
    void saveWindowSize();
    bool constructorFinished {false};
    
    LV_HeaderComponent headerComponent;
    LV_FilterComponent filterComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustaFilterAudioProcessorEditor)
};
