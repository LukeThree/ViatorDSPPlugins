/*
  ==============================================================================

    ButtonProps.cpp
    Created: 10 Jun 2022 8:09:32pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void  VibeMechinicAudioProcessorEditor::setTextButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(button);
    button.setClickingTogglesState(true);
    
    if (button.getComponentID() == "settings")
    {
        button.setColour(juce::ComboBox::outlineColourId, m_textAccentColor);
        button.setColour(juce::TextButton::ColourIds::textColourOnId, m_textAccentColor);
        button.setColour(juce::TextButton::ColourIds::textColourOffId, m_textAccentColor);
        button.setColour(juce::TextButton::ColourIds::buttonColourId, m_mainCompColor);
        button.setColour(juce::TextButton::ColourIds::buttonOnColourId, m_mainCompColor);
        button.setLookAndFeel(&customButtonLAF);
    }
    
    else
    {
        button.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
        button.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::whitesmoke);
        button.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::whitesmoke.withAlpha(0.5f));
        button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGB(53, 55, 70));
        button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB(53, 55, 70).brighter(0.1));
    }
    
    /** Individual Button Props */
    setSettingsButtonProps();
}

void  VibeMechinicAudioProcessorEditor::setSettingsButtonProps()
{
    m_settingsButton.setComponentID("settings");
    m_settingsButton.onClick = [this]()
    {
        resized();
        repaint();
        
        m_settingsPage.setEnabled(m_settingsButton.getToggleState());
        m_settingsPage.setVisible(m_settingsButton.getToggleState());
    };
}

void  VibeMechinicAudioProcessorEditor::setModuleToggleProps(viator_gui::Toggle &toggle)
{
    addAndMakeVisible(toggle);
    toggle.setToggleStyle(viator_gui::Toggle::ToggleStyle::kPower);
    toggle.setColour(juce::ToggleButton::ColourIds::tickColourId, m_mainCompColor);
}

void  VibeMechinicAudioProcessorEditor::setPrePostProps()
{

    addAndMakeVisible(m_prepostButton);
    prepostToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.m_treeState, midPreID, m_prepostButton);
    m_prepostButton.getToggleState() ? m_prepostButton.setButtonText("Post") : m_prepostButton.setButtonText("Pre");
    m_prepostButton.onClick = [this]()
    {
        m_prepostButton.getToggleState() ? m_prepostButton.setButtonText("Post") : m_prepostButton.setButtonText("Pre");
    };
    
    m_prepostButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
    m_prepostButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
    m_prepostButton.setColour(juce::TextButton::ColourIds::textColourOffId, findColour(juce::ToggleButton::tickDisabledColourId));
    m_prepostButton.setColour(juce::TextButton::ColourIds::textColourOnId, m_mainCompColor);
    m_prepostButton.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::transparentBlack);
}

void  VibeMechinicAudioProcessorEditor::setNavButtonProps(juce::TextButton& button)
{
    addAndMakeVisible(button);
    button.setClickingTogglesState(false);
    
    button.setColour(juce::ComboBox::outlineColourId, m_mainCompColor.withAlpha(0.4f));
    button.setColour(juce::TextButton::ColourIds::textColourOnId, m_textColor);
    button.setColour(juce::TextButton::ColourIds::textColourOffId, m_textColor);
    button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
    button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
    button.setLookAndFeel(&customNavButton);
}
