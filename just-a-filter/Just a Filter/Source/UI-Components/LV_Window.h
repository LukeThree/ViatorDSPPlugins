/*
  ==============================================================================

    LV_Window.h
    Created: 23 Oct 2021 12:53:38am
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LV_Window  : public juce::Component
{
public:
    LV_Window();
    ~LV_Window() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setWidthAndHeight(float w, float h);
    
private:
    
    juce::Image background;
    
    /** Vars ==================================================================*/
    float width {0.0f};
    float height {0.0f};
    bool paintInitialized = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV_Window)
};
