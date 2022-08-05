/*
  ==============================================================================

    PatreonComp.h
    Created: 30 May 2022 7:04:29pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PatreonComp  : public juce::Component
{
public:
    PatreonComp();
    ~PatreonComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //Patreon link
    juce::HyperlinkButton mWebLink;
    juce::URL mWebUrl {"https://www.patreon.com/ViatorDSP?fan_landing=true"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatreonComp)
};
