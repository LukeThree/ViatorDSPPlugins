/*
  ==============================================================================

    MultiBandWindow.cpp
    Created: 4 Dec 2022 2:38:09pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MultiBandWindow.h"

//==============================================================================
MultiBandWindow::MultiBandWindow(SweetenerAudioProcessor& p) : audioProcessor(p)
{
    _bandSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    _bandSlider.setLookAndFeel(&_windowSliderStyle);
    cutoffAttach = std::make_unique<juce::TwoValueSliderAttachment>(audioProcessor.treeState, "cutoff1", "cutoff2", _bandSlider);

    addAndMakeVisible(_bandSlider);
    
    _bandSlider.onValueChange = [this]()
    {
        _bandSlider.setPopupPosition();
    };
}

MultiBandWindow::~MultiBandWindow()
{
    _bandSlider.setLookAndFeel(nullptr);
}

void MultiBandWindow::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.brighter(0.1));
    drawGrid(g);
    drawTextLabels(g);
}

void MultiBandWindow::resized()
{
    _bandSlider.setBounds(0, 0, getWidth(), getHeight());
}

std::vector<float> MultiBandWindow::getFrequencies()
{
    return std::vector<float>
    {
        20, /*30, 40,*/ 50, 100,
        200, /*300, 400,*/ 500, 1000,
        2000, /*3000, 4000,*/ 5000, 10000,
        20000
    };
}

std::vector<float> MultiBandWindow::getGains()
{
    return std::vector<float>
    {
        -24, -12, 0, 12, 24
    };
}

std::vector<float> MultiBandWindow::getXs(const std::vector<float> &freqs, float left, float width)
{
    std::vector<float> xs;
    for( auto f : freqs )
    {
        auto normX = juce::mapFromLog10(f, 20.f, 20000.f);
        xs.push_back( left + width * normX );
    }
    
    return xs;
}

void MultiBandWindow::drawGrid(juce::Graphics &g)
{
    using namespace juce;
    auto freqs = getFrequencies();
    
    auto renderArea = getLocalBounds().reduced(10, 8);
    auto left = renderArea.getX();
    auto right = renderArea.getRight();
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();
    
    auto xs = getXs(freqs, left, width);
    
    g.setColour(Colours::dimgrey.withAlpha(0.5f));
    for( auto x : xs )
    {
        g.drawVerticalLine(x, top, bottom);
    }
    
    auto gain = getGains();
    
    for( auto gDb : gain )
    {
        auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        
        g.setColour(gDb == 0.f ? Colour(0u, 172u, 1u).withAlpha(0.5f) : Colours::darkgrey.withAlpha(0.5f) );
        g.drawHorizontalLine(y, left, right);
    }
}

void MultiBandWindow::drawTextLabels(juce::Graphics &g)
{
    using namespace juce;
    g.setColour(Colours::lightgrey);
    const int fontHeight = getWidth() * 0.0116;
    g.setFont(fontHeight);
    
    auto renderArea = getLocalBounds().reduced(10, 8);
    auto left = renderArea.getX();
    
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();
    
    auto freqs = getFrequencies();
    auto xs = getXs(freqs, left, width);
    
    for( int i = 0; i < freqs.size(); ++i )
    {
        auto f = freqs[i];
        auto x = xs[i];

        bool addK = false;
        String str;
        if( f > 999.f )
        {
            addK = true;
            f /= 1000.f;
        }

        str << f;
        if( addK )
            str << "k";
        str << "Hz";
        
        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;

        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
        r.setY(1);
        
        if (str != "20Hz" && str != "20kHz")
        {
            g.drawFittedText(str, r, juce::Justification::centred, 1);
        }
    }
    
    auto gain = getGains();

    for( auto gDb : gain )
    {
        auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        
        String str;
        if( gDb > 0 )
            str << "+";
        str << gDb;
        
        auto textWidth = g.getCurrentFont().getStringWidth(str);
        
        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(getWidth() - textWidth);
        r.setCentre(r.getCentreX(), y);
        
        g.setColour(gDb == 0.f ? Colour(0u, 172u, 1u) : Colours::lightgrey );
        
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
        
        str.clear();
        str << (gDb - 24.f);

        r.setX(1);
        textWidth = g.getCurrentFont().getStringWidth(str);
        r.setSize(textWidth, fontHeight);
        g.setColour(Colours::lightgrey);
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
    }
}
