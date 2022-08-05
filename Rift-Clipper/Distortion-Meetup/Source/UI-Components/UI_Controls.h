/*
  ==============================================================================

    UI_Controls.h
    Created: 9 Nov 2021 4:42:02pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../UI-Components/LV_DialComponent.h"
#include "../Widgets/LV_Menu.h"

//==============================================================================
/*
*/
class UI_Controls  : public juce::Component
{
public:
    UI_Controls(juce::AudioProcessorValueTreeState& tree);
    ~UI_Controls() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setBoundsConstraints(float boundsWidth, float boundsHeight);

private:
    
    LV_DialComponent inputDial;
    LV_DialComponent driveDial;
    LV_DialComponent crossOverDial;
    LV_DialComponent mixDial;
    LV_DialComponent outputDial;
    
    juce::Label inputLabel {"inputLabel", "Input"};
    juce::Label driveLabel {"driveLabel", "Curve"};
    juce::Label crossOverLabel {"crossOverLabel", "X-Over"};
    juce::Label mixLabel {"mixLabel", "Mix"};
    juce::Label outputLabel {"outputLabel", "Output"};
    
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    juce::LV_Menu bandMenu;
    std::unique_ptr<ComboBoxAttachment> bandMenuAttach;
    
    std::vector<LV_DialComponent*> dials
    {
      &inputDial, &driveDial, &crossOverDial, &mixDial, &outputDial
    };
    
    std::vector<juce::Label*> labels
    {
      &inputLabel, &driveLabel, &crossOverLabel, &mixLabel, &outputLabel
    };
    
    float width;
    float height;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UI_Controls)
};
