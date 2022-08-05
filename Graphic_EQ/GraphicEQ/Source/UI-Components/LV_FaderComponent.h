/*
  ==============================================================================

    LV_FaderComponent.h
    Created: 23 Oct 2021 2:38:27am
    Author:  Landon Viator
 
    In your main editor, make sure to call setColour first, then enableShadow, if not,
    the shadow won't show up.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Widgets/LV_Fader.h"
#include "../Widgets/StyleSheet.h"

//==============================================================================
/*
*/
class LV_FaderComponent  : public juce::Component
{
public:
    LV_FaderComponent(juce::String suffix, double rangeStart, double rangeEnd, double intervalValue, double returnValue);
    ~LV_FaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setColour(int colourID, juce::Colour newColour);
    void enableShadow(bool enable);
    
private:
    
    /** Slider ================================================================*/
    juce::LV_Fader slider;
    juce::LV_FaderLAF customFader;
    
    /** Fader shadow ==========================================================*/
    juce::DropShadow sliderShadowProperties;
    juce::DropShadowEffect sliderShadow;
    void init_shadows();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_FaderComponent)
};
