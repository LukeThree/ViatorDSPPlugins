/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./UI-Components/HeaderComponent.h"
#include "./UI-Components/SettingsPage.h"

//==============================================================================
/**
*/
class VoxDriveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VoxDriveAudioProcessorEditor (VoxDriveAudioProcessor&);
    ~VoxDriveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VoxDriveAudioProcessor& audioProcessor;
    
    /** Boiler plate UI stuff */
    void uiConstructor();
    void initWindow();
    void uiPaint(juce::Graphics &g);
    void uiResized();
    void saveWindowSize();
    bool constructorFinished = false;
    
    float topMargin;
    
    //Header
    HeaderComponent headerComponent;
    
    //Settings Page
    SettingsPage settingsPage;
    void setSettingsPageBounds();
    
    viator_gui::Dial driveDial;
    viator_gui::Dial rangeDial;
    viator_gui::Dial lowpassDial;
    viator_gui::Dial trimDial;
    viator_gui::Dial outDial;
    viator_gui::Dial mixFader;
    
    viator_gui::ToggleButton osButton;
    viator_gui::ToggleButton phaseButton;
    
    viator_gui::Label driveDialLabel {true, "Drive"};
    viator_gui::Label rangeDialLabel {true, "Range"};
    viator_gui::Label lowpassDialLabel {true, "Lowpass"};
    viator_gui::Label trimDialLabel {true, "Trim"};
    viator_gui::Label outDialLabel {true, "Output"};
    viator_gui::Label mixFaderLabel {true, "Mix"};
    
    bool imageAlphaChanged = false;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowpassAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trimAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttach;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> hqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttach;
    
    juce::String driveToolTip =
    {
        "This knob drives the input harder into the exciter circuit, which makes mid and upper range harmonic distortion. \nIncreasing the drive also boosts a hard-coded low-shelf EQ to compensate for perceived low-end loss."
    };
    
    juce::String rangeToolTip =
    {
        "This knob sets the cutoff of frequencies affected by the harmonic distortion. Any frequencies above this cutoff are affected by harmonic distortion."
    };
    
    juce::String lowpassToolTip =
    {
        "This dial sets the cutoff for a lowpass (high cut) filter."
    };
    
    juce::String trimToolTip =
    {
        "A volume control for the wet signal only."
    };
    
    juce::String outToolTip =
    {
        "A master output volume control for the entire plugin's output."
    };
    
    juce::String mixToolTip =
    {
        "The Mix fader mixes the uneffected input from the DAW with the effected processing from the plugin."
    };
    
    juce::String hqToolTip =
    {
        "This turns on oversampling, which raises the quality of the effect and avoids aliasing in the non-linear processing."
    };
    
    juce::String phaseToolTip =
    {
        "This flips the polarity of the audio."
    };
    
    juce::Rectangle<float> tooltipWindow;
    juce::Label tooltipContent;
    
    std::vector<juce::Slider*> sliders = {&driveDial, &rangeDial, &lowpassDial, &trimDial, &outDial, &mixFader};
    std::vector<juce::String> sliderLabels = {driveToolTip, rangeToolTip, lowpassToolTip, trimToolTip, outToolTip, mixToolTip};
    
    std::vector<juce::ImageButton*> buttons = {&osButton, &phaseButton};
    std::vector<juce::String> buttonLabels = {hqToolTip, phaseToolTip};
    
    std::vector<viator_gui::Label*> labels = {&driveDialLabel, &rangeDialLabel, &lowpassDialLabel, &trimDialLabel, &outDialLabel, &mixFaderLabel};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoxDriveAudioProcessorEditor)
};
