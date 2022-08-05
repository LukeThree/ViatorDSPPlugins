/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI-Components/LV_Window.h"
#include "Widgets/LV_Dial.h"
#include "Widgets/LV_Fader.h"

//==============================================================================
/**
*/
class MultibandstereowidenerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultibandstereowidenerAudioProcessorEditor (MultibandstereowidenerAudioProcessor&);
    ~MultibandstereowidenerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    MultibandstereowidenerAudioProcessor& audioProcessor;
    
    /** UI Methods ================================================================*/
    void uiConstructor();
    void uiPaint();
    void uiResized(float width, float height);
    
    /** Window ====================================================================*/
    LV_Window windowComponent;
    
    void initWindow();
    void saveWindowSize();
    bool constructorFinished {false};
    
    juce::LV_Dial lowWidthDial;
    juce::LV_Dial highWidthDial;
    juce::LV_Fader crossoverFader;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lowWidthDialAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> highWidthDialAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> crossoverDialAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultibandstereowidenerAudioProcessorEditor)
};
