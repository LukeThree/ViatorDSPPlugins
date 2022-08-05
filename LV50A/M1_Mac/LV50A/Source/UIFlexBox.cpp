/*
  ==============================================================================

    UIFlexBox.cpp
    Created: 27 May 2021 1:46:48pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void LV50AAudioProcessorEditor::setFlexBox()
{
    //Master bounds object
    juce::Rectangle<int> bounds = getLocalBounds();

    // Column 1
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnOne.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnOne.alignContent = juce::FlexBox::AlignContent::center;

    // Add additional sliders here
    juce::Array<juce::FlexItem> itemArrayColumnOne;
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandOneGainSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.07f, 0, 0, bounds.getWidth() * .05)));
    itemArrayColumnOne.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandOneFreqSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.08f, 0, 0, bounds.getWidth() * .05)));

    flexboxColumnOne.items = itemArrayColumnOne;
    flexboxColumnOne.performLayout(bounds.removeFromLeft(bounds.getWidth() / 5));
    
    lowShelfToggle.setBounds(bandOneFreqSlider.getX() + bandOneFreqSlider.getWidth() / 3, bandOneFreqSlider.getY() + bandOneFreqSlider.getHeight(), bandOneFreqSlider.getWidth() / 3, bandOneFreqSlider.getHeight() / 3);
    /* ============================================================================ */
    
    // Column 2
    juce::FlexBox flexboxColumnTwo;
    flexboxColumnTwo.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnTwo.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnTwo.alignContent = juce::FlexBox::AlignContent::center;

    // Add additional sliders here
    juce::Array<juce::FlexItem> itemArrayColumnTwo;
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandTwoGainSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.07f, 0, 0, 0)));
    itemArrayColumnTwo.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandTwoFreqSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.08f, 0, 0, 0)));

    flexboxColumnTwo.items = itemArrayColumnTwo;
    flexboxColumnTwo.performLayout(bounds.removeFromLeft(bounds.getWidth() / 5));
    
    // Reset button
    gainResetButton.setBounds(bandTwoFreqSlider.getX() + bandTwoFreqSlider.getWidth() * 0.72, bandTwoFreqSlider.getY() + bandTwoFreqSlider.getHeight(), bandTwoFreqSlider.getWidth() / 3, bandTwoFreqSlider.getHeight() / 3);
    /* ============================================================================ */
    
    // Column 3
    juce::FlexBox flexboxColumnThree;
    flexboxColumnThree.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnThree.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnThree.alignContent = juce::FlexBox::AlignContent::center;

    // Add additional sliders here
    juce::Array<juce::FlexItem> itemArrayColumnThree;
    itemArrayColumnThree.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandThreeGainSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.07f, 0, 0, 0)));
    itemArrayColumnThree.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandThreeFreqSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.08f, 0, 0, 0)));

    flexboxColumnThree.items = itemArrayColumnThree;
    flexboxColumnThree.performLayout(bounds.removeFromLeft(bounds.getWidth() / 4));
    /* ============================================================================ */
    
    // Column 4
    juce::FlexBox flexboxColumnFour;
    flexboxColumnFour.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnFour.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnFour.alignContent = juce::FlexBox::AlignContent::center;

    // Add additional sliders here
    juce::Array<juce::FlexItem> itemArrayColumnFour;
    itemArrayColumnFour.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandFourGainSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.07f, 0, 0, 0)));
    itemArrayColumnFour.add(juce::FlexItem(bounds.getWidth() / 4, bounds.getHeight() / 4, bandFourFreqSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.08f, 0, 0, 0)));

    flexboxColumnFour.items = itemArrayColumnFour;
    flexboxColumnFour.performLayout(bounds.removeFromLeft(bounds.getWidth() / 3));
    
    highShelfToggle.setBounds(bandFourFreqSlider.getX() + bandFourFreqSlider.getWidth() / 6, bandFourFreqSlider.getY() + bandFourFreqSlider.getHeight(), bandOneFreqSlider.getWidth() / 3, bandOneFreqSlider.getHeight() / 3);
    /* ============================================================================ */
    
    // Column 5
    juce::FlexBox flexboxColumnFive;
    flexboxColumnFive.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnFive.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnFive.alignContent = juce::FlexBox::AlignContent::center;

    // Add additional sliders here
    juce::Array<juce::FlexItem> itemArrayColumnFive;
    itemArrayColumnFive.add(juce::FlexItem(bounds.getWidth() / 3.0, bounds.getHeight() / 2.0, driveSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.15f, 0, 0, 0)));

    flexboxColumnFive.items = itemArrayColumnFive;
    flexboxColumnFive.performLayout(bounds.removeFromLeft(bounds.getWidth() / 2));
    
    filtersToggle.setBounds(driveSlider.getX(), lowShelfToggle.getY(), driveSlider.getWidth(), lowShelfToggle.getHeight());
    /* ============================================================================ */
    
    // Column 6
    juce::FlexBox flexboxColumnSix;
    flexboxColumnSix.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnSix.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxColumnSix.alignContent = juce::FlexBox::AlignContent::center;

    // Add additional sliders here
    juce::Array<juce::FlexItem> itemArrayColumnSix;
    itemArrayColumnSix.add(juce::FlexItem(bounds.getWidth() / 3.0, bounds.getHeight() / 2.0, trimSlider).withMargin(juce::FlexItem::Margin(bounds.getHeight() * 0.15f, 0, 0, 0)));

    flexboxColumnSix.items = itemArrayColumnSix;
    flexboxColumnSix.performLayout(bounds.removeFromLeft(bounds.getWidth()));
    
    phaseToggle.setBounds(trimSlider.getX() + trimSlider.getWidth() / 8, lowShelfToggle.getY(), lowShelfToggle.getWidth(), lowShelfToggle.getHeight());

    mSampleMenu.setBounds(filtersToggle.getX(), phaseToggle.getY() + phaseToggle.getHeight() * 2.0, phaseToggle.getWidth() * 2.0, phaseToggle.getHeight());
}
