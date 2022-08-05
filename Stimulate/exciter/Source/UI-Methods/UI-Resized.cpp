/*
  ==============================================================================

    UI-Resized.cpp
    Created: 24 Oct 2021 1:41:10am
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void ExciterAudioProcessorEditor::uiResized(float width, float height)
{
    auto leftMargin = height * 0.02;
    auto topMargin = height * 0.14;
    auto dialSize = width * 0.18;
    auto spaceBetween = width * 0.02;
    auto analyzerWidth = width * 0.33;
    auto faderWidth = width * 0.12;
    auto faderHeight = height * 0.8;
    
    // Plugin background UI
    header.setWidthAndHeight(width, height);
    header.setBounds(0, 0, width, height * 0.08f);
    
    // Plugin background UI
    amountDial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    rangeDial.setBounds(amountDial.getX() + amountDial.getWidth() + spaceBetween, topMargin, dialSize, dialSize);
    oddDial.setBounds(amountDial.getX(), amountDial.getY() + amountDial.getHeight() + topMargin * 0.75, dialSize, dialSize);
    evenDial.setBounds(rangeDial.getX(), oddDial.getY(), dialSize, dialSize);
    
    analyzer.setBounds(evenDial.getX() + evenDial.getWidth(), topMargin, analyzerWidth, getHeight() - topMargin * 2.0);
    spectrumWindow.setBounds(analyzer.getX(), analyzer.getY(), analyzer.getWidth(), analyzer.getHeight());
    phaseToggle.setBounds(analyzer.getX() + analyzer.getWidth() * 0.15, analyzer.getY() - topMargin * 0.36, 48, 48);
    osToggle.setBounds(analyzer.getX() + analyzer.getWidth() * 0.75, analyzer.getY() - topMargin * 0.36, 48, 48);
    mixFader.setBounds(analyzer.getX() + analyzer.getWidth(), topMargin, faderWidth, faderHeight);
    trimFader.setBounds(mixFader.getX() + mixFader.getWidth(), topMargin, faderWidth, faderHeight);
    
    // Save plugin size in the tree
    saveWindowSize();
}
