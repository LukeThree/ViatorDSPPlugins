/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ViatorDial.h"

//==============================================================================
/**
*/



class GainStagerAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener, juce::Button::Listener
{
public:
    GainStagerAudioProcessorEditor (GainStagerAudioProcessor&);
    ~GainStagerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

private:
    
    /**Set up the UI window size*/
    void setUpWindow(GainStagerAudioProcessor& audioProcessor);
    
    /**Bug with the resized function finishing before the constructor, this bool helps fix it*/
    bool constructorFinished = false;
    
    /**Image background*/
    juce::Image pluginBackground;
    
    // Shadow
    void setShadowProps();
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    // Logo
    juce::Image mLogo;
    
    // Sliders
    void setCommonSliderProps(juce::Slider& slider);
    juce::Slider preampSlider;
    std::vector<juce::Slider*> sliders {&preampSlider};
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> preampSliderAttach;
    
    // Labels
    void setCommonLabelProps(juce::Label& label);
    juce::Label preampLabel;
    std::vector<juce::Label*> labels {&preampLabel};
    
    // Buttons
    void setCommonButtonProps(juce::TextButton& button);
    juce::TextButton phaseToggle, resetToggle;
    std::vector<juce::TextButton*> buttons {&phaseToggle, &resetToggle};
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttach, resetAttach;
    

    // Set all component properties
    void setAllProps();
    
    // Window border
    juce::GroupComponent windowBorder;
        
    // Custom dial look and feel
    ViatorDial customDial;
    
    // Hyperlink button
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.patreon.com/ViatorDSP"};
    
    // Menu box
    juce::ComboBox mStageMenu;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> mStageMenuAttach;
       
    juce::ComboBox mCalibrationMenu;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> mCalibrationMenuAttach;
    
    GainStagerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainStagerAudioProcessorEditor)
};
