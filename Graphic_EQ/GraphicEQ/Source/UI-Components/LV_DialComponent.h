/*
  ==============================================================================

    LV_DialComponent.h
    Created: 24 Oct 2021 12:17:36am
    Author:  Landon Viator
 
    The last arg for the dial style is 1-3, you can also set the colors for each with the
    usual calls.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Widgets/LV_Dial.h"
#include "../Widgets/StyleSheet.h"

//==============================================================================
/*
*/
class LV_DialComponent  : public juce::Component
{
public:
    LV_DialComponent(juce::String suffix, double rangeStart, double rangeEnd, double intervalValue, double returnValue, int dialStyle);
    ~LV_DialComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setColour(int colourID, juce::Colour newColour);
    void enableShadow(bool enable);
    void setDialStyle(int dialStyle);

private:
    
    /** Slider ================================================================*/
    juce::LV_Dial dial;
    juce::LV_HardDialLAF hardDial;
    juce::LV_AlphaDialLAF alphaDial;
    juce::LV_CustomAbleDialLAF ableDial {false};
    
    /** Fader shadow ==========================================================*/
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    void init_shadows();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_DialComponent)
};
