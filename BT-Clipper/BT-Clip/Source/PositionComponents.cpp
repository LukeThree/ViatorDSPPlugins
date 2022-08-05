/*
  ==============================================================================

    PositionComponents.cpp
    Created: 9 Oct 2021 2:26:37am
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void BTClipAudioProcessorEditor::position_components()
{
    // Helpful vars
    float l_Width = AudioProcessorEditor::getWidth();
    float l_Height = AudioProcessorEditor::getHeight();
    float l_LeftMargin = l_Width * 0.1;
    float l_TopMargin = l_Height * 0.25;
    float l_DialSize = l_Width * 0.15;
    float l_SliderSize = l_Width * 0.1;
    
    // Sliders
    m_InputSlider.setBounds(l_LeftMargin, l_TopMargin, l_SliderSize, l_SliderSize * 2.25);
    m_OutputSlider.setBounds(l_LeftMargin + m_InputSlider.getWidth(), l_TopMargin, l_SliderSize, l_SliderSize * 2.25);
    
    // Dials
    for (auto i {0}; i < sliders.size(); i++)
    {
        if (i == 0)
        {
            sliders[i]->setBounds(m_OutputSlider.getX() + m_OutputSlider.getWidth() * 1.25, l_TopMargin, l_DialSize, l_DialSize);
        }
        
        else
        {
            sliders[i]->setBounds(sliders[i-1]->getX() + sliders[i-1]->getWidth() * 1.25, l_TopMargin, l_DialSize, l_DialSize);
        }
    }
    
    // Pre/Post Toggle
    m_PrePostToggle.setBounds(m_CutoffDial.getX(), m_CutoffDial.getY() + m_CutoffDial.getHeight() + 24.0, m_CutoffDial.getWidth(), m_CutoffDial.getHeight() / 4.0);
    
    // Phase Toggle
    m_Phase.setBounds(m_MidToneGain.getX(), m_PrePostToggle.getY(), m_MidToneGain.getWidth(), m_PrePostToggle.getHeight());
    
    // Clip Menu
    m_ClipMenu.setBounds(m_DriveDial.getX(), m_PrePostToggle.getY(), m_DriveDial.getWidth(), m_PrePostToggle.getHeight());
}
