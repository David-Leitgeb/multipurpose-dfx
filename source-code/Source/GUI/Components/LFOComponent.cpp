/*
  ==============================================================================

    LFOComponent.cpp
    Created: 5 Mar 2024 11:25:57am
    Author:  leitg

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOComponent.h"

//==============================================================================
LFOComponent::LFOComponent(juce::AudioProcessorValueTreeState& apvts, int lfoIndex) :
// Parameter attachment setup
lfoRateAttachment(apvts, "LFO" + static_cast<juce::String>(lfoIndex) + "RateHz", sliderLfoRate),
lfoDepthAttachment(apvts, "LFO" + static_cast<juce::String>(lfoIndex) + "Depth", sliderLfoDepth),
lfoRateModeAttachment(apvts, "LFO" + static_cast<juce::String>(lfoIndex) + "RateMode", buttonRateMode)
{
    // This variable is used to write the correct LFO number (ID) in the GUI
	lfoID = std::to_string(lfoIndex);

    // Combo box setup:
    comboBoxLfoShape.addItem("Sine", 1);
    comboBoxLfoShape.addItem("Triangle", 2);
    comboBoxLfoShape.addItem("Square", 3);
    comboBoxLfoShape.addItem("Saw", 4);
    comboBoxLfoShape.addItem("Sample&Hold", 5);

    comboBoxLfoRate.addItem("1", 1);
    comboBoxLfoRate.addItem("1/2", 2);
    comboBoxLfoRate.addItem("1/4", 3);
    comboBoxLfoRate.addItem("1/8", 4);
    comboBoxLfoRate.addItem("1/16", 5);
    comboBoxLfoRate.addItem("1/32", 6);

    // Combo box attachments have to be constructed after the items have been added.
    lfoShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "LFO" + static_cast<juce::String>(lfoIndex) + "Shape", comboBoxLfoShape);
    lfoRateSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "LFO" + static_cast<juce::String>(lfoIndex) + "RateSync", comboBoxLfoRate);

    // Setting the correct slider suffix.
    sliderLfoRate.setTextValueSuffix(" Hz");
    sliderLfoDepth.setTextValueSuffix(" %");

    // Making the individual components visible
    addAndMakeVisible(comboBoxLfoShape);
    addAndMakeVisible(sliderLfoRate);
    addAndMakeVisible(comboBoxLfoRate);
    addAndMakeVisible(buttonRateMode);
    addAndMakeVisible(sliderLfoDepth);
	addAndMakeVisible(labelDepth);
    addAndMakeVisible(labelRateHz);
    addAndMakeVisible(labelShape);
    addAndMakeVisible(labelRateSync);
    addAndMakeVisible(labelSyncMode);

    // Setting the correct label text justification.
    labelShape.setJustification(juce::Justification::centredLeft);
    labelRateSync.setJustification(juce::Justification::centredLeft);
    labelSyncMode.setJustification(juce::Justification::centredRight);
}

LFOComponent::~LFOComponent()
{
}

void LFOComponent::paint (juce::Graphics& g)
{
    // Bounds of the LFO component.
    juce::Rectangle<int> rectBounds = getLocalBounds();
    float cornerSize = rectBounds.getHeight() * 0.01f * 12.9f;

    // Background
    float trimSize = rectBounds.getWidth() * 0.002f * 4.0f;
    g.setColour(ThemeColorManager::mdmTrioDark);
    g.fillRoundedRectangle(rectBounds.withTrimmedTop(trimSize).withTrimmedBottom(trimSize).withTrimmedLeft(trimSize).withTrimmedRight(trimSize).toFloat(), cornerSize);
    g.setColour(ThemeColorManager::mdmTrioDark.brighter(0.1f));

    // LFO title text.
    g.setColour(ThemeColorManager::mdmTextWhite);
    g.setFont(juce::Font("Trebuchet MS", rectBounds.getHeight() * 0.1f, juce::Font::plain));
    g.drawFittedText("LFO" + lfoID, rectBounds.withTrimmedTop(rectBounds.getHeight() * 0.04f), juce::Justification::centredTop, 1);
}

void LFOComponent::resized()
{
    int controlsCount = 3;
    juce::Rectangle<int> rectBounds = getLocalBounds().reduced(getHeight() * 0.1f);

    int boundsHeight = rectBounds.getHeight();
    int boundsWidth = rectBounds.getWidth();
    int labelHeight = boundsHeight * 0.164f;

    // Dividing the component bounds into rectangles that are then set as the bounds of the individual components.
    juce::Rectangle<int> rectLfoDepth = rectBounds.removeFromRight(boundsWidth / controlsCount).withSizeKeepingCentre(rectBounds.getHeight() * 0.75f, rectBounds.getHeight() * 0.75f),
        rectLfoRateHz = rectBounds.removeFromRight(boundsWidth / controlsCount).withSizeKeepingCentre(rectBounds.getHeight() * 0.75f, rectBounds.getHeight() * 0.75f);

    juce::Rectangle<int>rectControls = rectBounds.withSizeKeepingCentre(rectBounds.getHeight() * 0.75f, rectBounds.getHeight() * 0.75f),
        rectLfoShape = rectControls.removeFromTop(rectControls.getHeight() * 0.5f),
        rectLfoRateSync = rectControls;
	
    // Setting the component bounds
    sliderLfoDepth.setBounds(rectLfoDepth);
    labelDepth.setBounds(rectLfoDepth.removeFromTop(labelHeight));
    sliderLfoRate.setBounds(rectLfoRateHz);
    labelRateHz.setBounds(rectLfoRateHz.removeFromTop(labelHeight));
    auto controlHeight = boundsHeight / 3;
    labelShape.setBounds(rectLfoShape.removeFromTop(labelHeight));
    comboBoxLfoShape.setBounds(rectLfoShape.removeFromTop(controlHeight));
    labelRateSync.setBounds(rectLfoRateSync.removeFromTop(controlHeight * 0.5f));
    comboBoxLfoRate.setBounds(rectLfoRateSync);
    juce::Rectangle<int> rectLfoSyncMode = getLocalBounds().removeFromBottom(boundsHeight * 0.24f).withSizeKeepingCentre(boundsWidth, labelHeight);
	labelSyncMode.setBounds(rectLfoSyncMode.removeFromLeft(boundsWidth * 0.5f));
    buttonRateMode.setBounds(rectLfoSyncMode.removeFromLeft(labelHeight).withSizeKeepingCentre(labelHeight * 0.75f, labelHeight * 0.75f));
}
