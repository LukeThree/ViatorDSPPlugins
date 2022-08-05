#include "SettingsButtonLAF.h"

SettingsButtonLAF::SettingsButtonLAF()
{
    
}

void SettingsButtonLAF::drawButtonBackground (juce::Graphics& g,
                                              juce::Button& button,
                                           const juce::Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto cornerSize = 6.0f;
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);
    
    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f).withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);
    
    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);
    
    g.setColour (baseColour);
    
    g.fillRoundedRectangle (bounds, cornerSize);
    
    g.setColour (button.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
    
    // Gear
    auto gear = juce::ImageCache::getFromMemory(BinaryData::gear01_png, BinaryData::gear01_pngSize);
    g.drawImageWithin(gear, 0, 0, bounds.getWidth(), bounds.getHeight(), juce::RectanglePlacement::stretchToFit);
}
