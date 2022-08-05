/*
  ==============================================================================

    DiscordComp.h
    Created: 30 May 2022 7:04:17pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DiscordComp  : public juce::Component
{
public:
    DiscordComp();
    ~DiscordComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    
    //Discord link
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://discord.gg/zcmjABjVn8"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiscordComp)
};
