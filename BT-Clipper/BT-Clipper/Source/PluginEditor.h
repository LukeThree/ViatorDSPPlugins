/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI-Components/LV_Window.h"
#include "UI-Components/LV_ClipperComponent.h"
#include "UI-Components/LV_PreToneComponent.h"
#include "UI-Components/LV_PostToneComponent.h"
#include "UI-Components/LV_HeaderComponent.h"
#include "Widgets/WidgetIncludes.h"

//==============================================================================
/**
*/
class BTClipperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BTClipperAudioProcessorEditor (BTClipperAudioProcessor&);
    ~BTClipperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    BTClipperAudioProcessor& audioProcessor;
    
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
    LV_PreToneComponent preToneComponent;
    LV_ClipperComponent clipperComponent;
    LV_PostToneComponent postComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BTClipperAudioProcessorEditor)
};
