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
class BedroomCompAudioProcessorEditor  :
public juce::AudioProcessorEditor,
private juce::Timer
{
public:
    BedroomCompAudioProcessorEditor (BedroomCompAudioProcessor&);
    ~BedroomCompAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() final
    {
        meterNeedleDial.setValue(audioProcessor.compressorModule.getCurrentSignal());
        
        if (meterNeedleDial.getValue() < -10.0)
        {
            meterNeedleDial.setValue(-10.0);
        }
        setRatioLabelValue();
        speedValueLabel.setText(audioProcessor.compressorModule.getSpeedValueString(), juce::dontSendNotification);
    }
    
   float float_one_point_round(float value);
    void setRatioLabelValue();
    
private:
    
    // Dials====================================================================================================================

    void setCommonDialProps(juce::Slider &slider);
    
    // Input Dials
    void initDials();
    void setinputDialProps();
    void setEQDialProps();
    void setCompressorDials();
    
    // Custom dial look and feel
    ViatorDial customDial;
    ViatorDial2 customMeterDial;
    
    juce::Slider
    
        inputDial, hpfDial, saturationDial, lowGainDial,
        midGainDial, highGainDial, lowFreqDial, midFreqDial,
        threshDial, ratioDial, compGainDial, speedDial, blendDial, meterNeedleDial;
    
    std::vector<juce::Slider*> dials
    {
        &inputDial, &hpfDial, &saturationDial, &lowGainDial,
        &midGainDial, &highGainDial, &lowFreqDial, &midFreqDial,
        &threshDial, &ratioDial, &compGainDial, &speedDial, &blendDial, &meterNeedleDial
    };
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment>
        inputDialAttach, hpfDialAttach, saturationDialAttach, lowGainDialAttach,
        midGainDialAttach, highGainDialAttach, lowFreqDialAttach, midFreqDialAttach,
        threshDialAttach, ratioDialAttach, compGainDialAttach, speedDialAttach, blendDialAttach;

    // Labels ====================================================================================================================

    void initLabels();
    void setCommonLabelProps(juce::Label &label);
    
    juce::Label
        inputDialLabel, hpfDialLabel, saturationDialLabel, lowGainDialLabel,
        midGainDialLabel, highGainDialLabel, lowFreqDialLabel, midFreqDialLabel,
        threshDialLabel, ratioDialLabel, compGainDialLabel, speedDialLabel, blendDialLabel, ratioValueLabel, speedValueLabel;
    
    std::vector<juce::Label*> labels
    {
        &inputDialLabel, &hpfDialLabel, &saturationDialLabel, &lowGainDialLabel,
        &midGainDialLabel, &highGainDialLabel, &lowFreqDialLabel, &midFreqDialLabel,
        &threshDialLabel, &ratioDialLabel, &compGainDialLabel, &speedDialLabel, &blendDialLabel
    };
    
    // Buttons ====================================================================================================================
    
    void setCommonButtonProps(juce::TextButton &button);
    void initButtons();
    void setEQButtonProps();
    void setCompButtonProps();
    void setPhaseToggleProps();
    void setSaturationTypeToggleProps();

    juce::TextButton
    
        phaseToggle, saturationTypeToggle, eqToggle, lowPeakToggle,
        midQToggle, highPeakToggle, highFreqToggle,
        compressorToggle, hpfSideChainToggle, fastModeToggle, gainBlendLinkToggle, preEQToggle;
    
    std::vector<juce::TextButton*> buttons
    {
        &phaseToggle, &saturationTypeToggle, &eqToggle, &lowPeakToggle,
        &midQToggle, &highPeakToggle, &highFreqToggle,
        &compressorToggle, &hpfSideChainToggle, &fastModeToggle, &gainBlendLinkToggle, &preEQToggle
    };
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment>
    
            phaseToggleAttach, saturationTypeToggleAttach, eqToggleAttach, lowPeakToggleAttach,
            midQToggleAttach, highPeakToggleAttach, highFreqToggleAttach,
            compressorToggleAttach, hpfSideChainToggleAttach, fastModeToggleAttach, gainBlendLinkToggleAttach, preEQToggleAttach;
        
    //====================================================================================================================

    void initBorders();
    void setBorderProps(juce::GroupComponent &border);
    juce::GroupComponent
    
            inputBorder, eqBorder, compressorBorder, outputBorder;
    
    std::vector<juce::GroupComponent*> borders
    {
        &inputBorder, &eqBorder, &compressorBorder, &outputBorder
    };
    
    // Shadow
    void setShadowProps();
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    // Plugin Header
    void setHeaderProps(juce::Graphics& g);
    
    // Plugin Background
    void setBackgroundProps(juce::Graphics& g);
    
    // Logo
    juce::Image mLogo;
    
    // Background
    juce::Image pluginBackground, vuMeter;
    
    // Window border
    juce::GroupComponent windowBorder;
        
    // Shadow button
    juce::ToggleButton shadowToggle;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> shadowToggleAttach;
        
    // Menu box
    void setOversampleMenuProps();
    juce::ComboBox mSampleMenu;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> mSampleMenuAttach;
        
    // Hyperlink button
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.patreon.com/ViatorDSP"};
    
    BedroomCompAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BedroomCompAudioProcessorEditor)
};
