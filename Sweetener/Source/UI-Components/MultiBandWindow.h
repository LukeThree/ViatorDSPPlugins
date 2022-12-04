/*
  ==============================================================================

    MultiBandWindow.h
    Created: 4 Dec 2022 2:38:09pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//==============================================================================
/*
*/
class MultiBandWindow  : public juce::Component
{
public:
    MultiBandWindow();
    ~MultiBandWindow() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    viator_gui::WindowSlider _bandSlider;
    juce::WindowSliderLAF _windowSliderStyle;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiBandWindow)
};
