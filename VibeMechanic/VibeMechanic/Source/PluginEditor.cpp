#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
 VibeMechinicAudioProcessorEditor:: VibeMechinicAudioProcessorEditor ( VibeMechanicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), m_settingsPage(audioProcessor)
, m_distortionLabel("Distortion")
, m_crusherLabel("Distortion")
, m_toneLabel("Tone")
, m_reverbLabel("Reverb")
{
    uiConstructor();
}

 VibeMechinicAudioProcessorEditor::~ VibeMechinicAudioProcessorEditor()
{
    /** Sliders */
    for (auto& slider : disSliders)
    {
        slider->setLookAndFeel(nullptr);
    }
    
    for (auto& slider : toneSliders)
    {
        slider->setLookAndFeel(nullptr);
    }
    
    for (auto& slider : verbSliders)
    {
        slider->setLookAndFeel(nullptr);
    }
     
     for (auto& button : navButtons)
     {
         button->setLookAndFeel(nullptr);
     }
    
    disSliders.clear();
    disSliders.shrink_to_fit();
    
    buttons.clear();
    buttons.shrink_to_fit();
    
    groups.clear();
    groups.shrink_to_fit();
    
    m_settingsButton.setLookAndFeel(nullptr);
     
     m_presetBrowser.setLookAndFeel(nullptr);
}

//==============================================================================
void  VibeMechinicAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void  VibeMechinicAudioProcessorEditor::resized()
{
    uiResized();
}


