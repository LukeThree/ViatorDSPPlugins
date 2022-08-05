/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BTClipAudioProcessorEditor::BTClipAudioProcessorEditor (BTClipAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    set_up_window(audioProcessor);
    init_dials();
    init_shadows();
    init_labels();
    init_toggles();
    init_Menu();
    set_phase_toggle_props();
    set_dial_props();
}

BTClipAudioProcessorEditor::~BTClipAudioProcessorEditor()
{
    clean_memory();
}

//==============================================================================
void BTClipAudioProcessorEditor::paint (juce::Graphics& g)
{
    paint_background(g);
    paint_logo(g);
}

void BTClipAudioProcessorEditor::resized()
{
    position_components();
    save_window_size();
}

void BTClipAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    // Drive compensation
    if (slider == &m_DriveDial)
    {
        drive_compensation();
    }
}
