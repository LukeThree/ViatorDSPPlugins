/*
  ==============================================================================

    CabButtonProps.cpp
    Created: 6 Jul 2021 6:08:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "PluginEditor.h"

void DiodeAmplifierAudioProcessorEditor::setCabButtonProps()
{
    
    cabButton.setClickingTogglesState(false);
    
    cabButton.onClick = [&]()
    {
        juce::FileChooser chooser ("Select an impulse response", audioProcessor.root, "*");
                
    
        
        if (chooser.browseForFileToOpen())
        {
            if (chooser.getResult().getFileExtension() == ".wav" || chooser.getResult().getFileExtension() == ".mp3")
            {
                audioProcessor.savedFile = chooser.getResult();
                
                audioProcessor.variableTree.setProperty("file", audioProcessor.savedFile.getFullPathName(), nullptr);
                audioProcessor.variableTree.setProperty("root", audioProcessor.savedFile.getParentDirectory().getFullPathName(), nullptr);

                audioProcessor.root = audioProcessor.savedFile.getParentDirectory().getFullPathName();

                audioProcessor.convolutionProcessor.loadImpulseResponse(audioProcessor.savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
                        
                DBG(audioProcessor.savedFile.getFullPathName());
            }
        
            else
            {
                settingsDialog.showNativeDialogBox("Error", "The file you chose is unsupported in some way. Select a .wav or .mp3 file.", false);
            }
        }
    };
    
    cabToggleButton.onClick = [&]()
    {
        const auto message = cabToggleButton.getToggleState() ? "Cab On" : "Cab Off";
                
        cabToggleButton.setButtonText(message);
    };
    
    resetIRButton.onClick = [&]()
    {
        audioProcessor.convolutionProcessor.loadImpulseResponse
        (BinaryData::metalOne_wav,
         BinaryData::metalOne_wavSize,
         juce::dsp::Convolution::Stereo::yes,
         juce::dsp::Convolution::Trim::yes, 0,
         juce::dsp::Convolution::Normalise::yes);
        
        audioProcessor.variableTree.setProperty("file", "/source/metalOne.wav", nullptr);
    };
}
