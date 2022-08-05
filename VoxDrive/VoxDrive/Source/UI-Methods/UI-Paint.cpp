#include "../PluginEditor.h"

void VoxDriveAudioProcessorEditor::uiPaint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    
    auto scale = 0.85f;
    auto leftMargin = getWidth() * (1.0 - scale) * 0.5;
    auto topMargin = getHeight() * (1.0 - scale * 0.9) * 0.5;
    
    //Init color
    masterColor = juce::Colour::fromString(audioProcessor.variableTree.getProperty("mastercolor").toString());
    
    // Background
    auto background = juce::ImageCache::getFromMemory
    (
        BinaryData::background_png,
        BinaryData::background_pngSize
    );
    
    g.drawImageWithin
    (
         background,
         leftMargin,
         topMargin,
         getWidth() * scale,
         getHeight() * scale,
         juce::RectanglePlacement::stretchToFit
    );
    
    driveDial.updateLabelColor(masterColor);
    rangeDial.updateLabelColor(masterColor);
    lowpassDial.updateLabelColor(masterColor);
    trimDial.updateLabelColor(masterColor);
    outDial.updateLabelColor(masterColor);
    mixFader.updateLabelColor(masterColor);
    osButton.updateLabelColor(masterColor);
    phaseButton.updateLabelColor(masterColor);
    
    for (int i = 0; i < labels.size(); ++i)
    {
        labels[i]->updateLabelColor(masterColor);
    }
    
    //Circuit
    auto circuitScale = 0.4f;
    auto circuit = juce::ImageCache::getFromMemory(BinaryData::VoxDriveCircuit_png, BinaryData::VoxDriveCircuit_pngSize);
    
    imageAlphaChanged = audioProcessor.variableTree.getProperty("alpha");
    
    if (!imageAlphaChanged)
    {
        circuit.multiplyAllAlphas(0.3f);
        imageAlphaChanged = true;
        audioProcessor.variableTree.setProperty("alpha", imageAlphaChanged, nullptr);
    }
    
    g.drawImageWithin
    (
        circuit,
        getWidth() * 0.09f,
        getHeight() * 0.2f,
        getWidth() * 0.59006f * circuitScale,
        getWidth() * circuitScale,
        juce::RectanglePlacement::stretchToFit
    );
    
}
