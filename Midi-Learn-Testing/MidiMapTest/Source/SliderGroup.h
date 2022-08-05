/*
  ==============================================================================

    SliderGroup.h
    Created: 18 Apr 2022 11:36:16am
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class SliderGroup  : public juce::Component, private juce::Timer, public juce::Slider::Listener
{
public:
    SliderGroup(MidiMapTestAudioProcessor& p);
    ~SliderGroup() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override
    {
        if (midiLearnMode)
        {
            midiMapLogic();
        }
        
        if (macroLearnMode)
        {
            macroMapLogic();
        }
    }
    
    /** Start and stop timer on midi learn button click*/
    void setMidiLearnMode(bool newLearnMode)
    {
        midiLearnMode = newLearnMode;
        
        if (midiLearnMode)
        {
            startTimerHz(30);
        }
        
        else
        {
            stopTimer();
            
            /** Deselect and turn off the learn mode*/
            for (const auto& slider : faders)
            {
                slider->setMidiSelectable(false);
                slider->setMidiSelected(false);
            }
        }
    }
    
    void midiMapLogic()
    {
        /** Set all sliders to be selectable if learn is on*/
        for (const auto& slider : faders)
        {
            slider->setMidiSelectable(true);

            /** Tell fader that is has a midi map based on the valuetree data*/
            for (int i = 0; i < audioProcessor.midiMap.midiParametersTree.getNumProperties(); i++)
            {
                if (slider->getComponentID() == audioProcessor.midiMap.midiParametersTree.getProperty(audioProcessor.midiMap.midiParametersTree.getPropertyName(i)).toString())
                {
                    slider->setHasMidiMap(true);
                }
            }

            /** Map midi control to a slider if it's selected by the user*/
            if (slider->getIsMidiSelected())
            {
                /** Assign map if unmapped */
                if (!slider->getHasMidiMap())
                {
                    audioProcessor.midiMap.setMidiGroup
                    (
                        audioProcessor.midiMap.getcurrentChannel(),
                        audioProcessor.midiMap.getCurrentCC(),
                        slider->getComponentID()
                    );

                    slider->setHasMidiMap(true);
                    slider->setMidiSelected(false);
                }

                /** Delete the mapping and then assign map*/
                else
                {
                    audioProcessor.midiMap.deleteMapping(slider->getComponentID());
                    audioProcessor.midiMap.setMidiGroup(audioProcessor.midiMap.getcurrentChannel(), audioProcessor.midiMap.getCurrentCC(), slider->getComponentID());
                    slider->setHasMidiMap(true);
                    slider->setMidiSelected(false);
                }
            }

            /** Delete mapping on right click*/
            if (slider->getHasMidiMap() && slider->getIsMidiMapToBeDeleted())
            {
                audioProcessor.midiMap.deleteMapping(slider->getComponentID());
                slider->setMidiMapToBeDeleted(false);
                slider->setHasMidiMap(false);
                slider->setMidiSelected(false);
            }
        }
    }
    
    /** Start and stop timer on midi learn button click*/
    void setMacroLearnMode(bool newLearnMode)
    {
        macroLearnMode = newLearnMode;
        
        if (macroLearnMode)
        {
            startTimerHz(30);
        }
        
        else
        {
            stopTimer();
            
            /** Deselect and turn off the learn mode*/
            for (const auto& slider : faders)
            {
                slider->setMacroSelectable(false);
                slider->setMacroSelected(false);
            }
        }
    }
    
    void macroMapLogic()
    {
        /** Set all sliders to be selectable if learn is on*/
        for (const auto& slider : faders)
        {
            slider->setMacroSelectable(true);

            /** Tell fader that is has a midi map based on the valuetree data*/
            for (int i = 0; i < audioProcessor.macroMap.macroParametersTree.getNumProperties(); i++)
            {
                if (lastSliderMovedID == audioProcessor.macroMap.macroParametersTree.getProperty(audioProcessor.macroMap.macroParametersTree.getPropertyName(i)).toString())
                {
                    if (slider->getComponentID() == lastSliderMovedID)
                    {
                        slider->setHasMacroMap(true);
                    }
                }
            }
            
            if (slider->getHasMacroMap() && slider->getIsMacroMapToBeDeleted())
            {
                audioProcessor.macroMap.deleteMapping(lastSliderMovedID);
                slider->setMacroMapToBeDeleted(false);
                slider->setHasMacroMap(false);
                slider->setMacroSelected(false);
            }
        }
            
            /** Map macro control to a slider if it's selected by the user*/
//            if (slider->getIsMacroSelected())
//            {
//                /** Assign map if unmapped */
//                if (!slider->getHasMacroMap())
//                {
//                    audioProcessor.macroMap.setMacro1Group(lastSliderMovedID);
//
//                    slider->setHasMacroMap(true);
//                    slider->setMacroSelected(false);
//                }
//
//                /** Delete the mapping and then assign map*/
//                else
//                {
//                    audioProcessor.macroMap.deleteMapping(lastSliderMovedID);
//                    audioProcessor.macroMap.setMacro1Group(lastSliderMovedID);
//                    slider->setHasMacroMap(true);
//                    slider->setMacroSelected(false);
//                }
//            }
//
//            /** Delete mapping on right click*/
        //}
    }
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    juce::String getLastSliderMovedID();

private:
    
    MidiMapTestAudioProcessor& audioProcessor;
    
    viator_gui::Fader fader1;
    viator_gui::Fader fader2;
    viator_gui::Fader fader3;
    viator_gui::Fader fader4;
    viator_gui::Border border;
    viator_gui::Dial macroDial;
    viator_gui::PushButton macroMapButton;
    
    std::vector<viator_gui::Fader*> faders =
    {
        &fader1, &fader2, &fader3, &fader4
    };
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p1Attach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p2Attach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p3Attach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p4Attach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> macroAttach;
    
    bool midiLearnMode = false;
    bool macroLearnMode = false;
    
    juce::String lastSliderMovedID;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderGroup)
};
