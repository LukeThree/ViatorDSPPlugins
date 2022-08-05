#include "../../PluginEditor.h"

void  VibeMechinicAudioProcessorEditor::uiConstructor()
{
    /** Plugin Window */
    initWindow();
    
    /** Settings Page and Plugin Theme*/
    addAndMakeVisible(m_settingsPage);
    m_settingsPage.setAlwaysOnTop(true);
    setPluginTheme(static_cast<Theme>(m_settingsPage.getPluginTheme()));
    
    /** Buttons */
    for (auto& button : buttons)
    {
        setTextButtonProps(*button);
    }
    
    driveToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.m_treeState, driveEnableID, m_distortionToggle);
    toneToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.m_treeState, toneEnableID, m_toneToggle);
    reverbToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.m_treeState, reverbEnableID, m_reverbToggle);
    
    
    /** Sliders */
    for (auto& slider : disSliders)
    {
        setSliderProps(*slider);
        
        if (slider != &m_driveInputDial)
        {
            slider->setEnabled(m_distortionToggle.getToggleState());
        }
    }
    
    for (auto& slider : toneSliders)
    {
        setSliderProps(*slider);
        slider->setEnabled(m_distortionToggle.getToggleState());
    }
    
    for (auto& slider : verbSliders)
    {
        setSliderProps(*slider);
        slider->setEnabled(m_reverbToggle.getToggleState());
    }
    
    setDisSliderProps();
    setToneSliderProps();
    setVerbDialProps();
    
    /** Groups */
    for (auto& group : groups)
    {
        setGroupProps(*group);
    }
    
    /** Module Toggles */
    for (auto& toggle : toggles)
    {
        setModuleToggleProps(*toggle);
    }
    
    setPrePostProps();
    
    /** Module Labels */
    for (auto& label : moduleLabels)
    {
        setModuleLabelProps(*label);
    }
    
    /** Module Menus */
    for (auto& menu : menus)
    {
        setMenuProps(*menu);
    }
    
    for (auto& button : navButtons)
    {
        setNavButtonProps(*button);
    }
    
    
    setDistortionMenuProps();
    setPresetBrowserItems();
    
    m_prevButton.setButtonText("<");
    m_prevButton.onClick = [this]()
    {
        if (m_presetBrowser.getSelectedItemIndex() > 0)
        {
            m_presetBrowser.setSelectedItemIndex(m_presetBrowser.getSelectedItemIndex() - 1);
        }
    };
    
    m_nextButton.setButtonText(">");
    m_nextButton.onClick = [this]()
    {
        if (m_presetBrowser.getSelectedItemIndex() < m_presetBrowser.getNumItems() - 1)
        {
            m_presetBrowser.setSelectedItemIndex(m_presetBrowser.getSelectedItemIndex() + 1);
        }
    };
    
    m_distortionToggle.onClick = [this]()
    {
        for (auto& slider : disSliders)
        {
            if (slider != &m_driveInputDial)
            {
                slider->setEnabled(m_distortionToggle.getToggleState());
            }
        }
    };
    
    m_toneToggle.onClick = [this]()
    {
        for (auto& slider : toneSliders)
        {
            slider->setEnabled(m_toneToggle.getToggleState());
        }
    };
    
    m_reverbToggle.onClick = [this]()
    {
        for (auto& slider : verbSliders)
        {
            slider->setEnabled(m_reverbToggle.getToggleState());
        }
    };
    
    m_mainBorder.setText("LV-Template");
}

void VibeMechinicAudioProcessorEditor::setPresetBrowserItems()
{
    m_presetBrowser.setTextWhenNothingSelected("Presets");
    m_presetBrowser.addItem("Lofi Room", 1);
    m_presetBrowser.addItem("Small Room", 2);
    m_presetBrowser.addItem("Live Room", 3);
    m_presetBrowser.addItem("Dry Recording", 4);
    m_presetBrowser.addItem("Vintage Console", 5);
    m_presetBrowser.addItem("Modern Console", 6);
    m_presetBrowser.addItem("Earthquake", 7);
    m_presetBrowser.addItem("Behind The Stage", 8);
    m_presetBrowser.addItem("Lofi Speaker", 9);
    m_presetBrowser.addItem("Lofi Vibes", 10);
    presetMenuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.m_treeState, presetID, m_presetBrowser);
    
    m_presetBrowser.onChange = [this]()
    {
        switch (m_presetBrowser.getSelectedItemIndex())
        {
            case 0:
            {
                // Lofi Room
                setPreset(1, 20.0f, 1.0f, 0.0f, 0, 1, 8.6f, 8.9f, 505.0f, 2.20, 1, 1, 0.1f, 1.0f, 0.0f, 0.3); break;
            }
                
            case 1:
            {
                // Small Room
                setPreset(1, 6.28f, 1.0f, 0.0f, 2, 1, 1.6f, 1.49f, 1426.0f, -3.9f, 1, 1, 0.08f, 0.58f, 0.27f, 0.2); break;
            }
                
            case 2:
            {
                // Live Room
                setPreset(1, 20.0f, 0.3f, -7.9f, 4, 1, 0.0f, 2.8f, 1281.0f, 0.0f, 0, 1, 0.4f, 0.35f, 1.0f, 0.15); break;
            }
                
            case 3:
            {
                // Dry Recording
                setPreset(1, 20.0f, 1.0f, 0.0f, 0, 1, 5.5f, 6.0f, 420.0f, 4.0f, 1, 0, 0.1f, 1.0f, 0.0f, 0.3); break;
            }
                
            case 4:
            {
                // Vintage Console
                setPreset(1, 20.0f, 0.6f, 0.0f, 2, 1, 0.0f, 8.45f, 728.0f, -11.0f, 1, 0, 0.2f, 0.6f, 1.0f, 0.4); break;
            }
                
            case 5:
            {
                // Modern Console
                setPreset(1, 9.9f, 0.5f, -6.0f, 1, 1, -2.07f, 2.74f, 5000.0f, 0.0f, 0, 0, 0.2f, 0.6f, 1.0f, 0.4); break;
            }
                
            case 6:
            {
                // Earthquake
                setPreset(1, 20.0f, 1.0f, -1.26f, 3, 1, 8.85f, 1.3f, 980.0f, 2.0f, 1, 1, 0.2f, 0.6f, 1.0f, 0.4); break;
            }
                
                
            case 7:
            {
                // Behind The Stage
                setPreset(1, 20.0f, 1.0f, -2.25f, 5, 1, -9.24f, -15.0f, 985.0f, -2.7f, 1, 1, 0.8f, 1.0f, 1.0f, 0.3); break;
            }
                
            case 8:
            {
                // Lofi Speaker
                setPreset(1, 20.0f, 1.0f, 2.29f, 0, 1, 8.85, -15.0f, 5000.0f, -2.7f, 1, 1, 0.0f, 1.0f, 0.0f, 0.15); break;
            }
                
            case 9:
            {
                // Lofi Vibes
                setPreset(1, 9.82f, 0.5f, -11.84f, 4, 1, -5.82, 5.68f, 1565.0f, 0.0f, 0, 1, 0.59f, 1.0f, 0.5f, 0.15); break;
            }
        }
    };
}

void VibeMechinicAudioProcessorEditor::setPreset(int newDisToggle, float newDrive, float newDriveMix, float newDriveOut, int newDriveType,
               int newToneToggle, float newTilt, float newTone, float newCutoff, float newToneOut, int newPrePost,
               int newVerbToggle, float newSize, float newDamp, float newWidth, float newBlend)
{
    audioProcessor.m_treeState.getParameterAsValue(driveEnableID) = newDisToggle;
    audioProcessor.m_treeState.getParameterAsValue(driveID) = newDrive;
    audioProcessor.m_treeState.getParameterAsValue(mixID) = newDriveMix;
    audioProcessor.m_treeState.getParameterAsValue(disOutputID) = newDriveOut;
    audioProcessor.m_treeState.getParameterAsValue(driveMenuID) = newDriveType;
    
    audioProcessor.m_treeState.getParameterAsValue(toneEnableID) = newToneToggle;
    audioProcessor.m_treeState.getParameterAsValue(midPreID) = newPrePost;
    audioProcessor.m_treeState.getParameterAsValue(tiltGainID) = newTilt;
    audioProcessor.m_treeState.getParameterAsValue(midCutoffID) = newCutoff;
    audioProcessor.m_treeState.getParameterAsValue(toneOutID) = newToneOut;
    audioProcessor.m_treeState.getParameterAsValue(midGainID) = newTone;
    
    audioProcessor.m_treeState.getParameterAsValue(roomSizeID) = newSize;
    audioProcessor.m_treeState.getParameterAsValue(dampingID) = newDamp;
    audioProcessor.m_treeState.getParameterAsValue(widthID) = newWidth;
    audioProcessor.m_treeState.getParameterAsValue(reverbMixID) = newBlend;
    audioProcessor.m_treeState.getParameterAsValue(reverbEnableID) = newVerbToggle;
}
