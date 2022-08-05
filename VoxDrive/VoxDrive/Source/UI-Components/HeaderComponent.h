#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/SettingsButtonLAF.h"
#include "../PluginProcessor.h"
#include "../Globals.h"

//==============================================================================
class HeaderComponent  : public juce::Component
{
public:
    HeaderComponent(VoxDriveAudioProcessor&);
    ~HeaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    bool getSettingsButtonToggleState();
    void updateBGColor();
    
private:
    
    VoxDriveAudioProcessor& audioProcessor;
    
    SettingsButtonLAF customSettingsButtonLAF;
    
    juce::TextButton settingsButton;
    void settingsButtonProps();

    //Patreon link
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.patreon.com/ViatorDSP"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};
