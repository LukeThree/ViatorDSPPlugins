/*
  ==============================================================================

    DistortionPanel.cpp
    Created: 8 Jul 2022 4:30:10pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void  VibeMechinicAudioProcessorEditor::setPanelLayout()
{
    /** Panels */
    auto leftMargin = getWidth() * 0.025;
    auto topMargin = getHeight() * 0.08 * 1.3;
    auto panelWidth = getWidth() * 0.21;
    auto panelHeight = getHeight() * 0.85;
    auto panelSpaceBetween = panelWidth * 0.085;
    
    m_distortionBorder.setBounds(leftMargin, topMargin, panelWidth * 1.4, panelHeight);
    m_toneBorder.setBounds(m_distortionBorder.getX() + m_distortionBorder.getWidth() + panelSpaceBetween * 0.89, topMargin, panelWidth * 1.4, panelHeight);
    m_reverbBorder.setBounds(m_toneBorder.getX() + m_toneBorder.getWidth() + panelSpaceBetween * 0.98, topMargin, panelWidth * 1.6, panelHeight);
}

void  VibeMechinicAudioProcessorEditor::setDistortionGroupLayout()
{
    const auto titleSize = 0.1;
    const auto toggleTop = 1.08;
    const auto toggleSize = getWidth() * 0.04;
    const auto labelWidth = getWidth() * 0.12;
    const auto labelHeight = getHeight() * 0.05;
    const auto menuX = m_distortionBorder.getX() * 3.4;
    const auto menuY = m_distortionBorder.getY() * 2.05;
    const auto menuWidth = m_distortionBorder.getWidth() * 0.6;
    const auto menuHeight = m_distortionBorder.getHeight() * 0.08;
    const auto dialX = m_distortionBorder.getX() * 1.5;
    const auto dialY = m_distortionBorder.getY() * 3.0;
    const auto dialSize = m_distortionBorder.getWidth() * 0.5;
    
    m_distortionTitle.setBounds(m_distortionBorder.getX(), m_distortionBorder.getY(), m_distortionBorder.getWidth(), m_distortionBorder.getHeight() * titleSize);
    m_distortionToggle.setBounds(m_distortionBorder.getX(), m_distortionBorder.getY() * toggleTop, toggleSize, toggleSize);
    m_distortionLabel.setBounds(m_distortionToggle.getX() + m_distortionToggle.getWidth() * 2.25, m_distortionToggle.getY() * 1.17, labelWidth, labelHeight);
    m_distortionMenu.setBounds(menuX, menuY, menuWidth, menuHeight);
    m_driveDial.setBounds(dialX, dialY, dialSize, dialSize);
    m_mixDial.setBounds(dialX, m_driveDial.getY() + m_driveDial.getHeight(), dialSize, dialSize);
    m_distortionVolumeDial.setBounds(m_mixDial.getX() + m_mixDial.getWidth() * 0.88, m_mixDial.getY(), dialSize, dialSize);
    m_driveInputDial.setBounds(m_distortionVolumeDial.getX(), m_driveDial.getY(), dialSize, dialSize);
}

void  VibeMechinicAudioProcessorEditor::setToneGroupLayout()
{
    const auto titleSize = 0.1;
    const auto toggleTop = 1.08;
    const auto toggleSize = getWidth() * 0.04;
    const auto labelWidth = getWidth() * 0.12;
    const auto labelHeight = getHeight() * 0.05;
    const auto dialX = m_toneBorder.getX();
    const auto dialY = m_toneBorder.getY() * 2.6;
    const auto dialSize = m_toneBorder.getWidth() * 0.5;
    const auto buttonX = m_toneBorder.getX() * 1.66;
    const auto buttonY = m_toneBorder.getY() * 1.18;
    const auto buttonWidth = m_toneBorder.getWidth() * 0.22;
    const auto buttonHeight = buttonWidth * 0.5;
    
    m_toneTitle.setBounds(m_toneBorder.getX(), m_toneBorder.getY(), m_toneBorder.getWidth(), m_toneBorder.getHeight() * titleSize);
    m_toneToggle.setBounds(m_toneBorder.getX(), m_toneBorder.getY() * toggleTop, toggleSize, toggleSize);
    m_toneLabel.setBounds(m_toneToggle.getX() + m_toneToggle.getWidth() * 2.1, m_toneToggle.getY() * 1.17, labelWidth, labelHeight);
    m_prepostButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
    
    m_tiltSlider.setBounds(dialX, dialY, dialSize, dialSize);
    m_toneDial.setBounds(m_tiltSlider.getX() + m_tiltSlider.getWidth(), m_tiltSlider.getY(), dialSize, dialSize);
    m_cutoffDial.setBounds(dialX, m_toneDial.getY() + m_toneDial.getHeight(), dialSize, dialSize);
    m_toneOutDial.setBounds(m_toneDial.getX(), m_cutoffDial.getY(), dialSize, dialSize);
}

void  VibeMechinicAudioProcessorEditor::setReverbGroupLayout()
{
    const auto titleSize = 0.1;
    const auto toggleTop = 1.08;
    const auto toggleSize = getWidth() * 0.04;
    const auto labelWidth = getWidth() * 0.12;
    const auto labelHeight = getHeight() * 0.05;
    const auto dialX = m_reverbBorder.getX() * 1.025;
    const auto dialY = m_reverbBorder.getY() * 2.5;
    const auto dialSize = m_reverbBorder.getWidth() * 0.45;
    
    m_reverbTitle.setBounds(m_reverbBorder.getX(), m_reverbBorder.getY(), m_reverbBorder.getWidth(), m_reverbBorder.getHeight() * titleSize);
    m_reverbToggle.setBounds(m_reverbBorder.getX(), m_reverbBorder.getY() * toggleTop, toggleSize, toggleSize);
    m_reverbLabel.setBounds(m_reverbToggle.getX() + m_reverbToggle.getWidth() * 2.59, m_reverbToggle.getY() * 1.17, labelWidth, labelHeight);
    
    m_sizeDial.setBounds(dialX, dialY, dialSize, dialSize);
    m_widthDial.setBounds(dialX, m_sizeDial.getY() + m_sizeDial.getHeight(), dialSize, dialSize);
    m_dampDial.setBounds(m_sizeDial.getX() + m_sizeDial.getWidth(), m_sizeDial.getY(), dialSize, dialSize);
    m_blendDial.setBounds(m_dampDial.getX(), m_dampDial.getY() + m_dampDial.getHeight(), dialSize, dialSize);
}
