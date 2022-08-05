/*
  ==============================================================================

    GraphicEQComponent.h
    Created: 12 Apr 2022 7:10:23pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class GraphicEQComponent  : public juce::Component
{
public:
    GraphicEQComponent(MultiQAudioProcessor& p);
    ~GraphicEQComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void reset(bool shouldReset);

private:
    
    MultiQAudioProcessor& audioProcessor;
    
    /** Sliders and Labels */
    viator_gui::Fader band1 {" dB", "31", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band1Label;
    
    viator_gui::Fader band2 {" dB", "63", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band2Label;
    
    viator_gui::Fader band3 {" dB", "125", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band3Label;
    
    viator_gui::Fader band4 {" dB", "250", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band4Label;
    
    viator_gui::Fader band5 {" dB", "500", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band5Label;
    
    viator_gui::Fader band6 {" dB", "1K", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band6Label;
    
    viator_gui::Fader band7 {" dB", "2K", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band7Label;
    
    viator_gui::Fader band8 {" dB", "4K", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band8Label;
    
    viator_gui::Fader band9 {" dB", "8K", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band9Label;
    
    viator_gui::Fader band10 {" dB", "15K", -12.0, 12.0, 0.01, 0.0};
    viator_gui::Label band10Label;
    
    /** Slider Attachments */
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> band1FaderAttach;
    std::unique_ptr<SliderAttachment> band2FaderAttach;
    std::unique_ptr<SliderAttachment> band3FaderAttach;
    std::unique_ptr<SliderAttachment> band4FaderAttach;
    std::unique_ptr<SliderAttachment> band5FaderAttach;
    std::unique_ptr<SliderAttachment> band6FaderAttach;
    std::unique_ptr<SliderAttachment> band7FaderAttach;
    std::unique_ptr<SliderAttachment> band8FaderAttach;
    std::unique_ptr<SliderAttachment> band9FaderAttach;
    std::unique_ptr<SliderAttachment> band10FaderAttach;
    
    /** Container for Faders */
    std::vector<viator_gui::Fader*> faders =
    {
        &band1, &band2, &band3, &band4, &band5, &band6, &band7, &band8, &band9, &band10
    };
    
    /** Container for Labels */
    std::vector<viator_gui::Label*> labels =
    {
        &band1Label, &band2Label, &band3Label, &band4Label, &band5Label, &band6Label, &band7Label, &band8Label, &band9Label, &band10Label
    };
    
    /** Container for Text */
    std::vector<juce::String> labelTexts =
    {
        "31", "63", "125", "250", "500", "1K", "2K", "4K", "8K", "15K"
    };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicEQComponent)
};
