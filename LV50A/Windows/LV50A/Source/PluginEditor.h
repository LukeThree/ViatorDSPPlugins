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
#include "./Widgets/StyleSheet.h"

//==============================================================================
/**
*/
class LV50AAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener, juce::Button::Listener
{
public:
    LV50AAudioProcessorEditor (LV50AAudioProcessor&);
    ~LV50AAudioProcessorEditor() override;

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
    
    /** Fader shadow ==========================================================*/
    juce::DropShadow sliderShadowProperties;
    juce::DropShadowEffect sliderShadow;
    void init_shadows();
    
    // Logo
    juce::Image mLogo;
    
    // Background
    juce::Image pluginBackground;

    
    // Output Sliders
    void setCommonVerticalSliderProps(juce::Slider& slider);
    
    juce::Slider driveSlider, trimSlider;
    std::vector<juce::Slider*> verticalSliders {&driveSlider, &trimSlider};
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveSliderAttach, trimSliderAttach;
    juce::LV_FaderLAF customFader;
    
    // Output Labels
    void setCommonLabelProps(juce::Label& label);
    
    juce::Label driveLabel, trimLabel;
    std::vector<juce::Label*> outLabels {&driveLabel, &trimLabel};
    std::vector<std::string> outLabelTexts {"Drive", "Trim"};
    
    // Filter Gain Sliders
    void setCommonFilterGainSliderProps(juce::Slider& slider);
    
    juce::Slider bandOneGainSlider, bandTwoGainSlider, bandThreeGainSlider, bandFourGainSlider;
    std::vector<juce::Slider*> filterSliders {&bandOneGainSlider, &bandTwoGainSlider, &bandThreeGainSlider, &bandFourGainSlider};
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> bandOneGainSliderAttach, bandTwoGainSliderAttach, bandThreeGainSliderAttach, bandFourGainSliderAttach;
    
    // Filter Labels
    juce::Label bandOneGainLabel, bandTwoGainLabel, bandThreeGainLabel, bandFourGainLabel;
    std::vector<juce::Label*> filterLabels {&bandOneGainLabel, &bandTwoGainLabel, &bandThreeGainLabel, &bandFourGainLabel};
    std::vector<std::string> filterLabelsTexts {"LF", "MF1", "MF2", "HF"};
    
    // Filter Freq Sliders
    void setCommonFilterFreqSliderProps(juce::Slider& slider);
    
    juce::Slider bandOneFreqSlider, bandTwoFreqSlider, bandThreeFreqSlider, bandFourFreqSlider;
    std::vector<juce::Slider*> filterFreqSliders {&bandOneFreqSlider, &bandTwoFreqSlider, &bandThreeFreqSlider, &bandFourFreqSlider};
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> bandOneFreqSliderAttach, bandTwoFreqSliderAttach, bandThreeFreqSliderAttach, bandFourFreqSliderAttach;
    
    // Buttons
    void setCommonButtonProps(juce::TextButton& button);
    juce::TextButton phaseToggle, lowShelfToggle, highShelfToggle, filtersToggle, gainResetButton;
    std::vector<juce::TextButton*> buttons {&phaseToggle, &lowShelfToggle, &highShelfToggle, &filtersToggle, &gainResetButton};
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttach, lowShelfToggleAttach, highShelfToggleAttach, filtersToggleAttach;

    // Set all component properties
    void setAllProps();
    
    void setFlexBox();
    
    // Shadow button
    juce::ToggleButton shadowToggle;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> shadowToggleAttach;
        
    // Custom dial look and feel
    ViatorDial customDial;
    ViatorDial2 customDial2;
    
    // Menu box
    juce::ComboBox mSampleMenu;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> mSampleMenuAttach;
    
    // Hyperlink button
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.patreon.com/ViatorDSP"};
    
    /** Window ====================================================================*/
    LV_Window windowComponent;
    
    void initWindow();
    void saveWindowSize();
    bool constructorFinished {false};
    
    /** UI Methods ================================================================*/
    void uiConstructor();
    void uiPaint();
    void uiResized(float width, float height);
    
    LV50AAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV50AAudioProcessorEditor)
};
