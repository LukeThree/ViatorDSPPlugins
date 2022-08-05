/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
GainStagerAudioProcessorEditor::GainStagerAudioProcessorEditor (GainStagerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Window size setup
    setUpWindow(audioProcessor);
    
    // Initialize shadows
    setShadowProps();

    // Initialize component properties
    setAllProps();

    // Slider one
    preampSlider.setTextValueSuffix(" dB");
    preampSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, preampSliderId, preampSlider);
    preampSlider.setRange(-100.0f, 100.0f, 0.25);

    // Label one
    preampLabel.setText("Trim", juce::dontSendNotification);
    preampLabel.attachToComponent(&preampSlider, false);


    // Phase button
    phaseToggle.setButtonText("Phase");
    phaseToggle.setClickingTogglesState(true);
    phaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseId, phaseToggle);
    

    // Reset button
    resetToggle.setButtonText("Listen");
    resetToggle.setClickingTogglesState(true);
    resetToggle.addListener(this);
    resetAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, listenId, resetToggle);

    // Window border
    addAndMakeVisible(windowBorder);
    windowBorder.setText("Einstein Stager");
    windowBorder.setColour(0x1005400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));
    windowBorder.setColour(0x1005410, juce::Colour::fromFloatRGBA(1, 1, 1, 0.25f));

    // Hyperlink button
    mWebLink.setURL(mWebUrl);
    addAndMakeVisible(mWebLink);
    
    // Menus
    addAndMakeVisible(&mStageMenu);
    mStageMenu.setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey.withAlpha(0.0f));
    mStageMenu.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke.withAlpha(0.35f));
    mStageMenu.setColour(juce::ComboBox::outlineColourId, juce::Colours::whitesmoke.withAlpha(0.35f));
    mStageMenu.setColour(juce::ComboBox::arrowColourId, juce::Colours::whitesmoke.darker(1.0f));
    mStageMenu.getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black.brighter(0.12));
    mStageMenu.getLookAndFeel().setColour(juce::PopupMenu::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    mStageMenu.getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::whitesmoke.withAlpha(0.05f));
    mStageMenu.setText("Stage Type");
    mStageMenu.addItem("Stage: Average", 1);
    mStageMenu.addItem("Stage: Peak", 2);
    mStageMenuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, stageTypeId, mStageMenu);

    addAndMakeVisible(&mCalibrationMenu);
    mCalibrationMenu.setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey.withAlpha(0.0f));
    mCalibrationMenu.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke.withAlpha(0.35f));
    mCalibrationMenu.setColour(juce::ComboBox::outlineColourId, juce::Colours::whitesmoke.withAlpha(0.35f));
    mCalibrationMenu.setColour(juce::ComboBox::arrowColourId, juce::Colours::whitesmoke.darker(1.0f));
    mCalibrationMenu.getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black.brighter(0.12));
    mCalibrationMenu.getLookAndFeel().setColour(juce::PopupMenu::textColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    mCalibrationMenu.getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::whitesmoke.withAlpha(0.05f));
    mCalibrationMenu.setText("Calibration");
    mCalibrationMenu.addItem("Level: -18", 1);
    mCalibrationMenu.addItem("Level: -21", 2);
    mCalibrationMenu.addItem("Level: -15", 3);
    mCalibrationMenu.addItem("Level: -12", 4);

    mCalibrationMenuAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, calibrationId, mCalibrationMenu);
}

void GainStagerAudioProcessorEditor::setUpWindow(GainStagerAudioProcessor &audioProcessor)
{
    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
    
    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();
    
    // Plugin window will always initialize to half the screen width by half of that for a rectangle
    auto width = x / 2.0;
    auto height = width / 2.0;
        
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width * 0.75, height * 0.75, width * 1.25, height * 1.25);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
        
    if (audioProcessor.windowWidth != 0.0)
    {
        setSize(audioProcessor.windowWidth, audioProcessor.windowHeight);
    }
        
    else
    {
        setSize (width, height);
    }
    
    constructorFinished = true;
}

GainStagerAudioProcessorEditor::~GainStagerAudioProcessorEditor()
{
    sliders.reserve(0);
    labels.reserve(0);
    buttons.reserve(0);
    resetToggle.removeListener(this);
    
    //This fixes a memory leak when you close the window
    for (auto i = 0; i < sliders.size(); ++i) {

        sliders[i]->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void GainStagerAudioProcessorEditor::paint (juce::Graphics& g)
{
    //Image layer from Illustrator
    pluginBackground = juce::ImageCache::getFromMemory(BinaryData::pluginBackground2_png, BinaryData::pluginBackground2_pngSize);
    
    // Draw and position the image
    g.drawImageWithin(pluginBackground, 0, 0, AudioProcessorEditor::getWidth(), AudioProcessorEditor::getHeight(), juce::RectanglePlacement::stretchToFit);

    // Logo
    mLogo = juce::ImageCache::getFromMemory(BinaryData::landon5504_png, BinaryData::landon5504_pngSize);
    g.drawImageWithin(mLogo, AudioProcessorEditor::getWidth() * 0.38, AudioProcessorEditor::getHeight() * 0.8 + 4, AudioProcessorEditor::getWidth() * 0.25, AudioProcessorEditor::getHeight() * 0.1, juce::RectanglePlacement::centred);
    

    // Web link
    mWebLink.setBounds(AudioProcessorEditor::getWidth() * 0.38, AudioProcessorEditor::getHeight() * 0.8 + 4, AudioProcessorEditor::getWidth() * 0.25, AudioProcessorEditor::getHeight() * 0.1);
}

void GainStagerAudioProcessorEditor::resized()
{
    
    double mWidth = AudioProcessorEditor::getWidth();
    double mHeight = AudioProcessorEditor::getHeight();
    double mLeftMargin = mWidth * 0.05;
    double mTopMargin = mHeight * 0.2;

    preampSlider.setBounds(mLeftMargin, mTopMargin, mWidth * 0.5, mHeight * 0.5);
    
    phaseToggle.setBounds(preampSlider.getX() + preampSlider.getWidth(), preampSlider.getY(), mWidth * 0.12, mWidth * 0.06);
    
    resetToggle.setBounds(phaseToggle.getX() + phaseToggle.getWidth() * 1.25, phaseToggle.getY(), mWidth * 0.12, mWidth * 0.06);
    
    mStageMenu.setBounds(
                         phaseToggle.getX(),
                         resetToggle.getY() + resetToggle.getHeight() * 1.25,
                         phaseToggle.getWidth() * 2.25,
                         mWidth * 0.06);
    
    mCalibrationMenu.setBounds(
                               mStageMenu.getX(),
                               mStageMenu.getY() + mStageMenu.getHeight() * 1.25,
                               mStageMenu.getWidth(),
                               mWidth * 0.06);

    windowBorder.setBounds
    (
        mWidth * 0.1,
        mHeight * 0.1,
        mWidth * 0.8,
        mHeight * .62
    );
    
    // Save plugin size in value tree
    audioProcessor.variableTree.setProperty("width", getWidth(), nullptr);
    audioProcessor.variableTree.setProperty("height", getHeight(), nullptr);
}

void GainStagerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    
}

void GainStagerAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &resetToggle)
    {
        if (resetToggle.getToggleState())
        {
            resetToggle.triggerClick();
        }
    }
}
