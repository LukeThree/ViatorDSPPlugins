#include <JuceHeader.h>
#include "SettingsPage.h"

//==============================================================================

SettingsPage::SettingsPage(VoxDriveAudioProcessor& p) : audioProcessor(p), masterColorComp(audioProcessor)
{
    setName("Settings Page");
    addAndMakeVisible(masterColorComp);
    addAndMakeVisible(discordComp);
    addAndMakeVisible(patreonComp);
    addAndMakeVisible(twitchComp);
}

SettingsPage::~SettingsPage()
{
    
}

void SettingsPage::paint (juce::Graphics& g)
{
    //Background
    g.fillAll(juce::Colours::black.withAlpha(0.7f));
    
    //Title Text
    g.setColour(juce::Colours::whitesmoke.withAlpha(0.5f));
    g.setFont(juce::Font ("Helvetica", getHeight() * 0.035, juce::Font::FontStyleFlags::bold));
    g.drawFittedText("Settings", getLocalBounds().withY(getHeight() * 0.02f), juce::Justification::centredTop, 1);
}

void SettingsPage::resized()
{
    rowheight = getHeight() * 0.075;
    
    masterColorComp.setBounds(0, rowheight, getWidth(), rowheight);
    discordComp.setBounds(0, masterColorComp.getY() + masterColorComp.getHeight(), getWidth(), rowheight);
    patreonComp.setBounds(0, discordComp.getY() + discordComp.getHeight(), getWidth(), rowheight);
    twitchComp.setBounds(0, patreonComp.getY() + patreonComp.getHeight(), getWidth(), rowheight);
}
