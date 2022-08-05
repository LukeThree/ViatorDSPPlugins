#pragma once
#include <JuceHeader.h>
#include "../../LookAndFeel/ColorButtonLAF.h"
#include "../../Globals.h"
#include "../../PluginProcessor.h"

//==============================================================================

class MasterColorComp  : public juce::Component
{
public:
    MasterColorComp(VoxDriveAudioProcessor&);
    ~MasterColorComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VoxDriveAudioProcessor& audioProcessor;
    
    juce::TextButton redButton;
    juce::TextButton greenButton;
    juce::TextButton blueButton;
    juce::TextButton orangeButton;
    juce::TextButton yellowButton;
    juce::TextButton purpleButton;
    juce::TextButton asphaltButton;
    juce::TextButton blackButton;
    
    ColorButtonLAF customColorButtonLAF;
    
    juce::Label colorsLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterColorComp)
};
