/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void LV50AAudioProcessorEditor::uiResized(float width, float height)
{
    // Plugin background UI
    windowComponent.setWidthAndHeight(width, height);
    windowComponent.setBounds(getLocalBounds());

    auto leftMargin = getWidth() * 0.1;
    auto topMargin = getHeight() * 0.08;
    auto sliderSize = getWidth() * 0.13;
    auto faderWidth = getWidth() * 0.08;
    auto faderHeight = getHeight() * 0.53;
    auto faderY = topMargin * 1.65;
    auto buttonWidth = sliderSize * 0.5;
    auto buttonHeight = getHeight() * 0.06;

    bandOneGainSlider.setBounds(leftMargin, topMargin, sliderSize, sliderSize);
    bandOneFreqSlider.setBounds(bandOneGainSlider.getX(), bandOneGainSlider.getY() + bandOneGainSlider.getHeight() * 1.25, sliderSize, sliderSize);

    bandTwoGainSlider.setBounds(bandOneGainSlider.getX() + bandOneGainSlider.getWidth(), topMargin, sliderSize, sliderSize);
    bandTwoFreqSlider.setBounds(bandTwoGainSlider.getX(), bandTwoGainSlider.getY() + bandTwoGainSlider.getHeight() * 1.25, sliderSize, sliderSize);

    bandThreeGainSlider.setBounds(bandTwoGainSlider.getX() + bandTwoGainSlider.getWidth(), topMargin, sliderSize, sliderSize);
    bandThreeFreqSlider.setBounds(bandThreeGainSlider.getX(), bandThreeGainSlider.getY() + bandThreeGainSlider.getHeight() * 1.25, sliderSize, sliderSize);

    bandFourGainSlider.setBounds(bandThreeGainSlider.getX() + bandThreeGainSlider.getWidth(), topMargin, sliderSize, sliderSize);
    bandFourFreqSlider.setBounds(bandFourGainSlider.getX(), bandFourGainSlider.getY() + bandFourGainSlider.getHeight() * 1.25, sliderSize, sliderSize);

    driveSlider.setBounds(bandFourGainSlider.getX() + bandFourGainSlider.getWidth() * 1.5, faderY, faderWidth, faderHeight);
    trimSlider.setBounds(driveSlider.getX() + driveSlider.getWidth() * 1.5, faderY, faderWidth, faderHeight);
    
    lowShelfToggle.setBounds(bandOneFreqSlider.getX() + bandOneFreqSlider.getWidth() * 0.25, bandOneFreqSlider.getY() + bandOneFreqSlider.getHeight(), buttonWidth, buttonHeight);
    gainResetButton.setBounds(bandTwoFreqSlider.getX() + bandTwoFreqSlider.getWidth() * 0.75, lowShelfToggle.getY(), buttonWidth, buttonHeight);
    highShelfToggle.setBounds(bandFourFreqSlider.getX() + bandFourFreqSlider.getWidth() * 0.25, bandOneFreqSlider.getY() + bandOneFreqSlider.getHeight(), buttonWidth, buttonHeight);

    filtersToggle.setBounds(driveSlider.getX(), driveSlider.getY() + driveSlider.getHeight(), driveSlider.getWidth(), buttonHeight);
    phaseToggle.setBounds(trimSlider.getX(), trimSlider.getY() + trimSlider.getHeight(), trimSlider.getWidth(), buttonHeight);

    mSampleMenu.setBounds(filtersToggle.getX(), phaseToggle.getY() + phaseToggle.getHeight() * 2.75, phaseToggle.getWidth() * 2.0, phaseToggle.getHeight());
    
    // Save plugin size in the tree
    saveWindowSize();
}
