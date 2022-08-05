/*
  ==============================================================================

    TwitchComp.h
    Created: 1 Jun 2022 11:22:28pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TwitchComp  : public juce::Component
{
public:
    TwitchComp();
    ~TwitchComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //Patreon link
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.twitch.tv/dr_bruisin"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwitchComp)
};
