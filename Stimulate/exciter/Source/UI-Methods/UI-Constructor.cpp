/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 24 Oct 2021 1:40:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void ExciterAudioProcessorEditor::uiConstructor()
{
    audioProcessor.pushNextSampleIntoFifo(0.0f);
    
    startTimerHz(10);
    
    // Window
    initWindow();
    
    addAndMakeVisible(header);
    
    addAndMakeVisible(amountDial);
    amountDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, "input", amountDial);
    amountDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.brighter(0.15f));
    amountDial.forceShadow();
    
    addAndMakeVisible(rangeDial);
    rangeDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, "range", rangeDial);
    rangeDial.setSkewFactorFromMidPoint(5000.0);
    rangeDial.setDialStyle(viator_gui::Dial::DialStyle::kFullDialMirrow);
    rangeDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.brighter(0.15f));
    rangeDial.forceShadow();
    
    addAndMakeVisible(oddDial);
    oddDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, "odd", oddDial);
    
    addAndMakeVisible(evenDial);
    evenDialAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, "even", evenDial);
    evenDial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::palevioletred.darker(1.0).darker(0.3));
    evenDial.forceShadow();
    
    addAndMakeVisible(mixFader);
    mixFaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, "mix", mixFader);
    
    addAndMakeVisible(trimFader);
    trimFaderAttach = std::make_unique<SliderAttachment>(audioProcessor.treeState, "trim", trimFader);
    
    addAndMakeVisible(analyzer);
    
    addAndMakeVisible(phaseToggle);
    phaseToggle.setToggleStyle(viator_gui::Toggle::ToggleStyle::kPhase);
    phaseAttach = std::make_unique<ButtonAttachment>(audioProcessor.treeState, "phase", phaseToggle);
    
    addAndMakeVisible(osToggle);
    osToggle.setToggleStyle(viator_gui::Toggle::ToggleStyle::kPower);
    osAttach = std::make_unique<ButtonAttachment>(audioProcessor.treeState, "os", osToggle);
}
