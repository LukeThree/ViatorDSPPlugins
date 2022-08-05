/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void MultiQAudioProcessorEditor::uiConstructor()
{
    // Window
    initWindow();
    addAndMakeVisible(windowComponent);
    
    // Output
    for (int i = 0; i < faders.size(); ++i)
    {
        addAndMakeVisible(faders[i]);
        addAndMakeVisible(labels[i]);
        //labels[i]->setText(labelTexts[i], juce::dontSendNotification);
        labels[i]->attachToComponent(faders[i], false);
        labels[i]->setJustificationType(juce::Justification::centred);
    }
    
    auto labelFont = getWidth() * 0.065 * 0.2;
    hpFaderLabel.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    lpFaderLabel.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    driveFaderLabel.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    trimFaderLabel.setFont(juce::Font ("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    
    hpFaderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highpassID, hpFader);
    lpFaderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowpassID, lpFader);
    driveFaderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveID, driveFader);
    trimFaderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimID, trimFader);
    
    // Choice buttons
    // Graphic
    addAndMakeVisible(graphicEQButton);
    graphicButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, graphicEQONID, graphicEQButton);
    graphicEQButton.setButtonText("Graphic");
    graphicEQButton.setRadioGroupId(1);
    graphicEQButton.onClick = [this]()
    {
        graphicEQComponent.setVisible(graphicEQButton.getToggleState());
        graphicEQComponent.setEnabled(graphicEQButton.getToggleState());
    };
    
    // Parametric
    addAndMakeVisible(paraEQButton);
    parametricButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, paraEQONID, paraEQButton);
    paraEQButton.setButtonText("Parametric");
    paraEQButton.setRadioGroupId(1);
    paraEQButton.onClick = [this]()
    {
        parametricEQComponent.setVisible(paraEQButton.getToggleState());
        parametricEQComponent.setEnabled(paraEQButton.getToggleState());
    };
    
    // Tube
    addAndMakeVisible(tubeEQButton);
    tubeButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, tubeEQONID, tubeEQButton);
    tubeEQButton.setButtonText("Tube");
    tubeEQButton.setRadioGroupId(1);
    tubeEQButton.onClick = [this]()
    {
        tubeEQComponent.setVisible(tubeEQButton.getToggleState());
        tubeEQComponent.setEnabled(tubeEQButton.getToggleState());
    };
    
    // EQ Components
    // Graphic EQ
    addAndMakeVisible(graphicEQComponent);
    graphicEQComponent.setVisible(graphicEQButton.getToggleState());
    graphicEQComponent.setEnabled(graphicEQButton.getToggleState());
    
    // Parametric EQ
    addAndMakeVisible(parametricEQComponent);
    parametricEQComponent.setVisible(paraEQButton.getToggleState());
    parametricEQComponent.setEnabled(paraEQButton.getToggleState());
    
    // Tube EQ
    addAndMakeVisible(tubeEQComponent);
    tubeEQComponent.setVisible(tubeEQButton.getToggleState());
    tubeEQComponent.setEnabled(tubeEQButton.getToggleState());
    
    addAndMakeVisible(resetButton);
    resetButton.setClickingTogglesState(false);
    resetButton.setButtonText("Reset");
    resetButton.onClick = [this]()
    {
        graphicEQComponent.reset(graphicEQButton.getToggleState());
        tubeEQComponent.reset(tubeEQButton.getToggleState());
        parametricEQComponent.reset(paraEQButton.getToggleState());
    };
}
