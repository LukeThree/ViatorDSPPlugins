/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LV_Dial.h"
#include "StyleSheet.h"
#include "LV_Label.h"
#include "LV_Fader.h"
#include "LV_Toggle.h"
#include "LV_GroupComponent.h"


//==============================================================================
/**
*/
class Pastel_VerbAudioProcessorEditor  :
public juce::AudioProcessorEditor,
public juce::Slider::Listener
{
public:
    Pastel_VerbAudioProcessorEditor (Pastel_VerbAudioProcessor&);
    ~Pastel_VerbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    /**Slider listener override*/
    void sliderValueChanged(juce::Slider *slider) override;
    
private:
    
    /**Set up the UI window size*/
    void setUpWindow(Pastel_VerbAudioProcessor& audioProcessor);
    void saveWindowSize();
    
    /**Image background*/
    juce::Image pluginBackground;
    
    /**Logo image*/
    juce::Image headerLogo;
    
    /**Bug with the resized function finishing before the constructor, this bool helps fix it*/
    bool constructorFinished = false;
    
    /**UI Widgets*/
    /**Sliders*/
    juce::LV_Dial cutoffSlider, resonanceSlider, driveSlider, roomSizeSlider, dampingSlider, widthSlider;
    juce::LV_Fader drySlider, wetSlider;
    juce::LV_FaderLAF customFader;
    juce::LV_HardDialLAF customDial;
    
    /**Slider attachments*/
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttach, resonanceSliderAttach, driveSliderAttach,
    roomSizeSliderAttach, dampingSliderAttach, widthSliderAttach, drySliderAttach, wetSliderAttach;
    
    /**Initialize the sliders*/
    void initSliders();
    
    /**Set individual slider props*/
    void setSliderProps();
    
    /**Container for all sliders*/
    std::vector<juce::Slider*> sliders =
    {
        &cutoffSlider, &resonanceSlider, &driveSlider, &roomSizeSlider, &dampingSlider, &widthSlider, &drySlider, &wetSlider
    };
        
    /**Buttons*/
    juce::LV_Toggle filterEngageButton, filterModeButton;
    
    /**Button attachments*/
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> filterEngageButtonAttach, filterModeButtonAttach;
    
    /**Initialize the buttons*/
    void initButtons();
    
    /**Individual button props*/
    void setSetFilterEngageButtonProps(juce::TextButton &button);
    void setSetFilterModeButtonProps(juce::TextButton &button);
    
    /**Labels*/
    juce::LV_Label cutoffLabel, resonanceLabel, driveLabel, roomLabel, dampingLabel, widthLabel, dryLabel, wetLabel;
        
    /**Initialize labels*/
    void initLabels();
        
    /**Container for labels*/
    std::vector<juce::Label*> dialLabels =
    {
        &cutoffLabel, &resonanceLabel, &driveLabel, &roomLabel,
        &dampingLabel, &widthLabel, &dryLabel, &wetLabel
    };
        
    /**Text for the labels*/
    std::vector<std::string> dialLabelTexts
    {
        "Cutoff", "Resonance", "Drive", "Room Size", "Damping", "Width", "Dry", "Wet"
    };
    
    /**Component border*/
    juce::LV_GroupComponent filterBorder;
    
    /**Initialize border*/
    void initFilterBorder();
    
    /**Shadows*/
    /**Initialize shadows*/
    void initShadows();
    
    /**Dial shadow*/
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    /**Fader shadow*/
    juce::DropShadow sliderShadowProperties;
    juce::DropShadowEffect sliderShadow;
        
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Pastel_VerbAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pastel_VerbAudioProcessorEditor)
};
