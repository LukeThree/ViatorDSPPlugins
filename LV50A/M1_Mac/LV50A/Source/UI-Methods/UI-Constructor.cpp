/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void LV50AAudioProcessorEditor::uiConstructor()
{
    // Window
    initWindow();
    addAndMakeVisible(windowComponent);
    
    // Initialize shadows
    setShadowProps();
    
    // Band sliders
    bandOneGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, gain1Id, bandOneGainSlider);
    bandOneGainSlider.setRange(-12.0, 12.0, 2.0);
    
    bandTwoGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, gain2Id, bandTwoGainSlider);
    bandTwoGainSlider.setRange(-12.0, 12.0, 2.0);
    
    bandThreeGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, gain3Id, bandThreeGainSlider);
    bandThreeGainSlider.setRange(-12.0, 12.0, 2.0);
    
    bandFourGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, gain4Id, bandFourGainSlider);
    bandFourGainSlider.setRange(-12.0, 12.0, 2.0);
    
    bandOneFreqSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, freq1Id, bandOneFreqSlider);
    bandOneFreqSlider.setRange(30.0, 400.0, 30.0);
    bandOneFreqSlider.setSkewFactorFromMidPoint(210.0);
    
    bandTwoFreqSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, freq2Id, bandTwoFreqSlider);
    bandTwoFreqSlider.setRange(200.0, 2000.0, 200.0);
    bandTwoFreqSlider.setSkewFactorFromMidPoint(1000.0);
    
    bandThreeFreqSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, freq3Id, bandThreeFreqSlider);
    bandThreeFreqSlider.setRange(200.0, 2000.0, 200.0);
    bandThreeFreqSlider.setSkewFactorFromMidPoint(1000.0);
    
    bandFourFreqSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, freq4Id, bandFourFreqSlider);
    bandFourFreqSlider.setRange(1000.0, 20000.0, 2000.0);
    bandFourFreqSlider.setSkewFactorFromMidPoint(7000.0);
    
    // Out Sliders
    driveSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveSliderId, driveSlider);
    driveSlider.setRange(0.0f, 12.0f, 0.5);
    driveSlider.setLookAndFeel(&customFader);
    
    trimSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    trimSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, trimSliderId, trimSlider);
    trimSlider.setRange(-24.0, 24.0, 1.0);
    trimSlider.setLookAndFeel(&customFader);
    
    // Initialize component properties
    setAllProps();
    
    driveSlider.setComponentEffect(&sliderShadow);
    trimSlider.setComponentEffect(&sliderShadow);
    
    // Buttons
    phaseToggle.setButtonText("Phase");
    phaseToggle.setClickingTogglesState(true);
    phaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseId, phaseToggle);
    
    lowShelfToggle.setButtonText("Shelf");
    lowShelfToggle.setClickingTogglesState(true);
    lowShelfToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, lowShelfBId, lowShelfToggle);
    
    highShelfToggle.setButtonText("Shelf");
    highShelfToggle.setClickingTogglesState(true);
    highShelfToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, highShelfBId, highShelfToggle);
    
    filtersToggle.setButtonText("Filters");
    filtersToggle.setClickingTogglesState(true);
    filtersToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, filtersBId, filtersToggle);
    
    gainResetButton.setButtonText("Reset");
    
    // Sample menu
    mSampleMenu.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black.withAlpha(0.0f));
    mSampleMenu.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    mSampleMenu.setColour(juce::ComboBox::outlineColourId, juce::Colours::whitesmoke.withAlpha(0.0f));
    mSampleMenu.setColour(juce::ComboBox::arrowColourId, juce::Colours::whitesmoke.darker(1.0f));
    mSampleMenu.getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black.brighter(0.12));
    mSampleMenu.getLookAndFeel().setColour(juce::PopupMenu::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    mSampleMenu.getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::whitesmoke.withAlpha(0.05f));
    mSampleMenu.setText("Oversampling");
    mSampleMenu.addItem("Oversample Off", 1);
    mSampleMenu.addItem("Oversample On", 2);
    mSampleMenuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, menuId, mSampleMenu);

    addAndMakeVisible(mSampleMenu);
    
    // Hyperlink button
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    
    // Gain reset button
    gainResetButton.addListener(this);
}
