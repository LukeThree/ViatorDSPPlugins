/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void GraphicEQAudioProcessorEditor::uiResized(float width, float height)
{
    // Plugin background UI
    windowComponent.setWidthAndHeight(width, height);
    windowComponent.setBounds(getLocalBounds());
    
    float leftMargin = width * 0.08f;
    float topMargin = height * 0.15f;
    float sliderWidth = leftMargin * 0.85f;
    float sliderHeight = height * 0.52f;
    float dialSize = leftMargin * 1.5f;
    
    for (auto i {0}; i < sliders.size() - 2; i++)
    {
        if (sliders[i] == &band1Slider)
        {
            sliders[i]->setBounds(leftMargin, topMargin, sliderWidth, sliderHeight);
        }
        
        else
        {
            sliders[i]->setBounds(sliders[i - 1]->getX() + sliders[i - 1]->getWidth(), topMargin, sliderWidth, sliderHeight);
        }
    }
    
    lowBellButton.setBounds(band2Slider.getX(), band2Slider.getY() + band2Slider.getHeight(), band2Slider.getWidth(), band2Slider.getWidth() * 0.5f);
    highBellButton.setBounds(band9Slider.getX(), band9Slider.getY() + band9Slider.getHeight(), band9Slider.getWidth(), band9Slider.getWidth() * 0.5f);

    driveSlider.setBounds(band10Slider.getX() + band10Slider.getWidth(), band10Slider.getY(), dialSize, dialSize);
    trimSlider.setBounds(band10Slider.getX() + band10Slider.getWidth(), band10Slider.getY() + driveSlider.getHeight() * 1.18f, dialSize, dialSize);
    
    phaseToggle.setBounds(trimSlider.getX(), highBellButton.getY(), trimSlider.getWidth(), highBellButton.getHeight());
    
    // Save plugin size in the tree
    saveWindowSize();
}
