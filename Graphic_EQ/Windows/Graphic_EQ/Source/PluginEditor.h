/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ViatorDial.h"
#include "UI-Components/LV_Window.h"
#include "Widgets/StyleSheet.h"

//==============================================================================
/**
*/
class GraphicEQAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener, juce::Button::Listener
{
public:
    GraphicEQAudioProcessorEditor (GraphicEQAudioProcessor&);
    ~GraphicEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

private:
    
    // Shadow
    void setShadowProps();
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    // Sliders
    void setCommonSliderProps(juce::Slider& slider);
    juce::Slider band1Slider, band2Slider, band3Slider, band4Slider, band5Slider, band6Slider, band7Slider, band8Slider, band9Slider, band10Slider, driveSlider, trimSlider;
    std::vector<juce::Slider*> sliders {&band1Slider, &band2Slider, &band3Slider, &band4Slider, &band5Slider, &band6Slider, &band7Slider, &band8Slider, &band9Slider, &band10Slider, &driveSlider, &trimSlider};
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> band1SliderAttach, band2SliderAttach, band3SliderAttach, band4SliderAttach, band5SliderAttach, band6SliderAttach, band7SliderAttach, band8SliderAttach, band9SliderAttach, band10SliderAttach, driveSliderAttach, trimSliderAttach;
    
    juce::LV_FaderLAF customFader;
    juce::LV_AlphaDialLAF customDial;
    
    // Labels
    void setCommonLabelProps(juce::Label& label);
    juce::Label band1Label, band2Label, band3Label, band4Label, band5Label, band6Label, band7Label, band8Label, band9Label, band10Label, driveLabel, trimLabel;
    std::vector<juce::Label*> labels {&band1Label, &band2Label, &band3Label, &band4Label, &band5Label, &band6Label, &band7Label, &band8Label, &band9Label, &band10Label, &driveLabel, &trimLabel};
    std::vector<std::string> labelTexts {"31", "63", "125", "250", "500", "1K", "2K", "4K", "8K", "15K", "Drive", "Trim"};
    
    // Buttons
    void setCommonButtonProps(juce::TextButton& button);
    juce::TextButton phaseToggle, lowBellButton, highBellButton;
    std::vector<juce::TextButton*> buttons {&phaseToggle, &lowBellButton, &highBellButton};
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttach, lowBellAttach, highBellAttach;
    std::vector<std::string> buttonTexts {"Phase", "Bell", "Bell"};
    


    // Set all component properties
    void setAllProps();
    
    // Window border
    juce::GroupComponent windowBorder;
    
    // Shadow button
    juce::ToggleButton shadowToggle;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> shadowToggleAttach;
        
    /** UI Methods ================================================================*/
    void uiConstructor();
    void uiPaint();
    void uiResized(float width, float height);
    
    /** Window ====================================================================*/
    LV_Window windowComponent;
    
    void initWindow();
    void saveWindowSize();
    bool constructorFinished {false};
    
    GraphicEQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicEQAudioProcessorEditor)
};
