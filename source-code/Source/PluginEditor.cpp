/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiBandModulatorAudioProcessorEditor::MultiBandModulatorAudioProcessorEditor(MultiBandModulatorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    // Initialization of class instances.
	globalInputGainAttachment(p.apvts, "GlobalInputGain", sliderInputGain),
    globalOutputGainAttachment(p.apvts, "GlobalOutputGain", sliderOutputGain),
    globalMixAttachment(p.apvts, "GlobalMix", sliderPluginMix),
    globalFeedforwardAttachment(p.apvts, "GlobalFeedforward", sliderGlobalFeedforward),
	bandComponentManager(p.apvts),
	LFO1Component(p.apvts, 1),
    LFO2Component(p.apvts, 2),
    LFO3Component(p.apvts, 3),
	headerComponent(p.getPresetManager())
{
    // GUI window settings.
    constexpr int windowWidth = 1280;
    constexpr int windowHeight = 640;
    setSize(windowWidth, windowHeight);
    // Window resize settings.
	constexpr float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    setResizable(false, true);
	getConstrainer()->setFixedAspectRatio(aspectRatio);
    setResizeLimits(640, 320, 1920, 960);

	createGUI();

    // Setting the correct suffix for each slider.
    sliderInputGain.setTextValueSuffix(" dB");
    sliderOutputGain.setTextValueSuffix(" dB");
    sliderGlobalFeedforward.setTextValueSuffix(" %");
    sliderPluginMix.setTextValueSuffix(" %");
}

MultiBandModulatorAudioProcessorEditor::~MultiBandModulatorAudioProcessorEditor()
{
}

//==============================================================================
void MultiBandModulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background color
	g.fillAll(ThemeColorManager::mdmTrioDark.brighter(0.075f));

    // Drawing the rectangle background of the global parameters components.
    juce::Rectangle<int> rectWindow = getLocalBounds();
    rectWindow.removeFromTop(rectWindow.proportionOfHeight(0.055f)); // Removing the header part of the window.
    rectWindow.removeFromTop(rectWindow.proportionOfHeight(0.7f)); // Removing the band part of the window.
    const juce::Rectangle<int> rectGlobalControls = rectWindow.removeFromRight(rectWindow.getHeight()).reduced(rectWindow.getHeight() * 0.016);
	const float cornerSize = rectGlobalControls.toFloat().getHeight() * 0.01f * 12.9f;
    g.setColour(ThemeColorManager::mdmTrioDark);
    g.fillRoundedRectangle(rectGlobalControls.toFloat(), cornerSize);
}

void MultiBandModulatorAudioProcessorEditor::resized()
{
    // Defining the basic structure of the plugin window.
    juce::Rectangle<int> rectWindow = getLocalBounds(),
        rectHeader = rectWindow.removeFromTop(rectWindow.proportionOfHeight(0.055f)),
        rectBandControls = rectWindow.removeFromTop(rectWindow.proportionOfHeight(0.7f)),
        rectGlobalControls = rectWindow.removeFromRight(rectWindow.getHeight()).reduced(rectWindow.getHeight() * 0.016f);

    // Defining the LFO section of the plugin window.
    const float rectLfoSectionWidth = rectWindow.getWidth();
    juce::Rectangle<int> rectLFO1 = rectWindow.removeFromLeft(rectLfoSectionWidth / 3.0f),
        rectLFO2 = rectWindow.removeFromLeft(rectLfoSectionWidth / 3.0f),
        rectLFO3 = rectWindow;

    // Setting the correct bounds to each component.
    headerComponent.setBounds(rectHeader);
    bandComponentManager.setBounds(rectBandControls);
    LFO1Component.setBounds(rectLFO1);
    LFO2Component.setBounds(rectLFO2);
    LFO3Component.setBounds(rectLFO3);

    // Defining the bounds of individual GUI components.
    const float globalControlsWidth = rectGlobalControls.getWidth();
    const float globalControlsHeight = rectGlobalControls.getHeight();
    const float labelHeight = (rectGlobalControls.getWidth() * 0.5f) * 0.3f;
    juce::Rectangle<int> rectInputGain = rectGlobalControls.withTrimmedBottom(globalControlsHeight * 0.5f).withTrimmedRight(globalControlsWidth * 0.5f),
        rectOutputGain = rectGlobalControls.withTrimmedBottom(globalControlsHeight * 0.5f).withTrimmedLeft(globalControlsWidth * 0.5f),
        rectPluginMix = rectGlobalControls.withTrimmedTop(globalControlsHeight * 0.5f).withTrimmedLeft(globalControlsWidth * 0.5f),
        rectGlobalFeedforward = rectGlobalControls.withTrimmedTop(globalControlsHeight * 0.5f).withTrimmedRight(globalControlsWidth * 0.5f);

    // Setting the bounds of each component.
    sliderInputGain.setBounds(rectInputGain);
	labelInputGain.setBounds(rectInputGain.removeFromTop(labelHeight).translated(0, -labelHeight * 0.2f));

    sliderOutputGain.setBounds(rectOutputGain);
    labelOutputGain.setBounds(rectOutputGain.removeFromTop(labelHeight).translated(0, -labelHeight * 0.2f));

    sliderPluginMix.setBounds(rectPluginMix);
    labelPluginMix.setBounds(rectPluginMix.removeFromTop(labelHeight).translated(0, -labelHeight * 0.2f));

    sliderGlobalFeedforward.setBounds(rectGlobalFeedforward);
    labelGlobalFeedforward.setBounds(rectGlobalFeedforward.removeFromTop(labelHeight).translated(0, -labelHeight * 0.2f));
}

// This method makes all the components visible at once.
void MultiBandModulatorAudioProcessorEditor::createGUI()
{
    addAndMakeVisible(headerComponent);
    addAndMakeVisible(sliderPluginMix);
    addAndMakeVisible(sliderOutputGain);
    addAndMakeVisible(sliderInputGain);
    addAndMakeVisible(sliderGlobalFeedforward);
    addAndMakeVisible(bandComponentManager);
    addAndMakeVisible(LFO1Component);
    addAndMakeVisible(LFO2Component);
    addAndMakeVisible(LFO3Component);

    addAndMakeVisible(labelInputGain);
    addAndMakeVisible(labelOutputGain);
    addAndMakeVisible(labelPluginMix);
    addAndMakeVisible(labelGlobalFeedforward);
}