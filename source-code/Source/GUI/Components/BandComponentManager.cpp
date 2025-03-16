/*
  ==============================================================================

    BandComponentManager.cpp
    Created: 31 Dec 2023 12:36:38pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BandComponentManager.h"

//==============================================================================
BandComponentManager::BandComponentManager(juce::AudioProcessorValueTreeState& apvts)
// Initialization of band component instances
    : band1Component(apvts, 1, selectedBandValue),
	  band2Component(apvts, 2, selectedBandValue),
      band3Component(apvts, 3, selectedBandValue),
      band4Component(apvts, 4, selectedBandValue),
      band5Component(apvts, 5, selectedBandValue),
      band6Component(apvts, 6, selectedBandValue),
      band7Component(apvts, 7, selectedBandValue),
      band8Component(apvts, 8, selectedBandValue),
      band9Component(apvts, 9, selectedBandValue),
      band10Component(apvts,10,selectedBandValue),
      band11Component(apvts, 11, selectedBandValue),
      band12Component(apvts, 12, selectedBandValue),
      band13Component(apvts, 13, selectedBandValue),
      band14Component(apvts, 14, selectedBandValue),
      band15Component(apvts, 15, selectedBandValue),
      band16Component(apvts, 16, selectedBandValue),
      band17Component(apvts, 17, selectedBandValue),
      band18Component(apvts, 18, selectedBandValue),
      band19Component(apvts, 19, selectedBandValue),
      band20Component(apvts, 20, selectedBandValue),
      band21Component(apvts, 21, selectedBandValue),
      band22Component(apvts, 22, selectedBandValue),
      band23Component(apvts, 23, selectedBandValue),
      band24Component(apvts, 24, selectedBandValue),
      band25Component(apvts, 25, selectedBandValue),
      band26Component(apvts, 26, selectedBandValue),
      band27Component(apvts, 27, selectedBandValue),
      band28Component(apvts, 28, selectedBandValue),
      band29Component(apvts, 29, selectedBandValue),
      band30Component(apvts, 30, selectedBandValue)
{

    // Setting the band components visible.
    for (auto* band : getBandComponents())
    {
        addAndMakeVisible(band);
    }

    // Setting up the view buttons.
    for (auto* button : getViewButtons())
    {
        addAndMakeVisible(button);
        button->setClickingTogglesState(true);
        button->setRadioGroupId(1001);
        button->addListener(this);
    }

    viewCutoffFreq.setToggleState(true, juce::dontSendNotification);

    // Setting up the modulation buttons.
    for (auto* button : getModulationButtons())
    {
        addAndMakeVisible(button);
        button->setClickingTogglesState(true);
        button->setRadioGroupId(1002);
    }
    modulationViewLFO1.setToggleState(true, juce::dontSendNotification);

    selectedBandValue.addListener(this);

    // Adjusting the colours for individual parameter view buttons.
    viewCutoffFreq.setColour(juce::TextButton::buttonColourId, ThemeColorManager::mdmViewCutoff);
    viewFilterQ.setColour(juce::TextButton::buttonColourId, ThemeColorManager::mdmViewQ);
    viewDelayTime.setColour(juce::TextButton::buttonColourId, ThemeColorManager::mdmViewDelay);
    viewFeedback.setColour(juce::TextButton::buttonColourId, ThemeColorManager::mdmViewFeedback);
    viewPanorama.setColour(juce::TextButton::buttonColourId, ThemeColorManager::mdmViewPanorama);
    viewBandGain.setColour(juce::TextButton::buttonColourId, ThemeColorManager::mdmViewGain);

    // Setting modulation components visible.
    addAndMakeVisible(viewModulation);
    addAndMakeVisible(modulationViewLabel);
    addAndMakeVisible(modulationViewLFO1);
    addAndMakeVisible(modulationViewLFO2);
    addAndMakeVisible(modulationViewLFO3);

    // Setting up parameters for the view components.
    viewModulation.setTooltip("This button switches between parameter/modulation view.");
    modulationViewLabel.setJustification(juce::Justification::centredLeft);
    modulationUpdateLabel.setJustification(juce::Justification::centredRight);

    // Setting up the modulation interval combo box.
    comboBoxModulationUpdateInterval.addItem("1", 1);
    comboBoxModulationUpdateInterval.addItem("8", 2);
    comboBoxModulationUpdateInterval.addItem("16", 3);
    comboBoxModulationUpdateInterval.addItem("32", 4);
    comboBoxModulationUpdateInterval.addItem("64", 5);
    comboBoxModulationUpdateInterval.addItem("128", 6);
    comboBoxModulationUpdateInterval.addItem("256", 7);
	modulationUpdateIntervalAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "ModulationUpdateInterval", comboBoxModulationUpdateInterval);

    // Setting up the modualtion interval components.
    addAndMakeVisible(modulationUpdateLabel);
	addAndMakeVisible(comboBoxModulationUpdateInterval);
    comboBoxModulationUpdateInterval.setTooltip("Changing this value adjusts the internal modulation update interval (in samples) used in processing. "
        "Choosing shorter intervals reduces artifacts, however it can significantly increase performance requirements.");

    // The following lambda functions handle the correct GUI behaviour.
    viewModulation.onClick = [this]()
    {
        modulationViewActivated = viewModulation.getToggleState();

        for (auto* band : getBandComponents())
        {
            band->changeVerticalSliderView(selectedView, modulationViewActivated, selectedLFO);
        }

        showModulationText();
    };

    modulationViewLFO1.onClick = [this]()
    {
        selectedLFO = 1;

        for (auto* band : getBandComponents())
        {
            band->changeVerticalSliderView(selectedView, modulationViewActivated, selectedLFO);
        }
    };

    modulationViewLFO2.onClick = [this]()
    {
        selectedLFO = 2;

        for (auto* band : getBandComponents())
        {
            band->changeVerticalSliderView(selectedView, modulationViewActivated, selectedLFO);
        }
    };

    modulationViewLFO3.onClick = [this]()
    {
        selectedLFO = 3;

        for (auto* band : getBandComponents())
        {
            band->changeVerticalSliderView(selectedView, modulationViewActivated, selectedLFO);
        }
    };

    // Text that is shown for parameters that cannot be modulated.
    textModNotAvailable.setText("MODULATION NOT AVAILABLE");
    textModNotAvailable.setColour(ThemeColorManager::mdmTrioColor);
    textModNotAvailable.setJustification(juce::Justification::centred);
    addChildComponent(textModNotAvailable);
}

BandComponentManager::~BandComponentManager()
{
}

void BandComponentManager::paint (juce::Graphics& g)
{
    // Background
    const juce::Rectangle<int> rectLocalBounds = getLocalBounds();
    g.setColour(ThemeColorManager::mdmTrioDark);
    g.fillRoundedRectangle(rectLocalBounds.withTrimmedTop(rectLocalBounds.getWidth() * 0.002f).withTrimmedBottom(rectLocalBounds.getWidth() * 0.002f).toFloat(), rectLocalBounds.getWidth() * 0.016f);
}

void BandComponentManager::resized()
{
    // Defining rectangles that correspond to individual GUI parts of the band component manager.
    juce::Rectangle<int> rectLocalBounds = getLocalBounds();
    const float cornerSize = (rectLocalBounds.toFloat().getWidth() * 0.016f);
    const float localBoundsHeight = rectLocalBounds.getHeight();
    const float viewAreaHeight = localBoundsHeight * 0.075f;
    juce::Rectangle<int> rectBands = rectLocalBounds.removeFromTop(getHeight() - viewAreaHeight),
		viewArea = rectLocalBounds.withTrimmedBottom(viewAreaHeight * 0.25f).removeFromBottom(viewAreaHeight).withTrimmedLeft(cornerSize).withTrimmedRight(cornerSize);

    // Text shown when modulation is not available.
    juce::Font textFont("Trebuchet MS", localBoundsHeight * 0.01f * 5.5f, juce::Font::bold);
    textModNotAvailable.setFont(textFont, true);
    textModNotAvailable.setBoundingBox(juce::Parallelogram(rectBands.withTrimmedTop(localBoundsHeight * 0.3f).toFloat()));

    // Setting the bounds for all band components.
	for (auto* band : getBandComponents())
    {
        band->setBounds(rectBands);
    }

    // Modulation controls
    const float viewButtonWidth = viewArea.getWidth() / 10;
    modulationViewLabel.setBounds(viewArea.removeFromLeft(viewButtonWidth * 0.7f));
    viewModulation.setBounds(viewArea.removeFromLeft(viewArea.getHeight()).withSizeKeepingCentre(viewArea.getHeight() * 0.65f, viewArea.getHeight() * 0.65f));
    auto rectViewButtons = viewArea.withTrimmedLeft(viewArea.getHeight() * 0.65f);
	modulationViewLFO1.setBounds(rectViewButtons.removeFromLeft(viewButtonWidth * 0.5f));
    modulationViewLFO2.setBounds(rectViewButtons.removeFromLeft(viewButtonWidth * 0.5f));
    modulationViewLFO3.setBounds(rectViewButtons.removeFromLeft(viewButtonWidth * 0.5f));
    modulationUpdateLabel.setBounds(rectViewButtons.removeFromLeft(viewButtonWidth * 0.81f));
    comboBoxModulationUpdateInterval.setBounds(rectViewButtons.removeFromLeft(viewButtonWidth * 0.5f));

    // View controls
    viewBandGain.setBounds(rectViewButtons.removeFromRight(viewButtonWidth));
    viewPanorama.setBounds(rectViewButtons.removeFromRight(viewButtonWidth));
    viewFeedback.setBounds(rectViewButtons.removeFromRight(viewButtonWidth));
    viewDelayTime.setBounds(rectViewButtons.removeFromRight(viewButtonWidth));
    viewFilterQ.setBounds(rectViewButtons.removeFromRight(viewButtonWidth));
    viewCutoffFreq.setBounds(rectViewButtons.removeFromRight(viewButtonWidth));
}

// This method changes the selected band.
void BandComponentManager::valueChanged(juce::Value&)
{
    for (auto* band : getBandComponents())
    {
        band->changeSelectedBand();
    }
}

// This method changes the view of the vertical section.
void BandComponentManager::buttonClicked(juce::Button* button)
{
    for (auto* band : getBandComponents())
    {
        band->changeVerticalSliderView(button->getButtonText(), modulationViewActivated, selectedLFO);
    }
    selectedView = button->getButtonText();

    showModulationText();
}


std::vector<BandComponent*> BandComponentManager::getBandComponents()
{
    return
    {
        &band1Component,
        &band2Component,
        &band3Component,
        &band4Component,
        &band5Component,
        &band6Component,
        &band7Component,
        &band8Component,
        &band9Component,
        &band10Component,
        &band11Component,
        &band12Component,
        &band13Component,
        &band14Component,
        &band15Component,
        &band16Component,
        &band17Component,
        &band18Component,
        &band19Component,
        &band20Component,
        &band21Component,
        &band22Component,
        &band23Component,
        &band24Component,
        &band25Component,
        &band26Component,
        &band27Component,
        &band28Component,
        &band29Component,
        &band30Component
    };
}

std::vector<juce::Button*> BandComponentManager::getViewButtons()
{
    return
    {
	    &viewCutoffFreq,
	    &viewFilterQ,
	    &viewPanorama,
	    &viewDelayTime,
	    &viewFeedback,
	    &viewBandGain
    };
}

std::vector<juce::Button*> BandComponentManager::getModulationButtons()
{
    return
    {
        &modulationViewLFO1,
        &modulationViewLFO2,
        &modulationViewLFO3
    };
}

void BandComponentManager::showModulationText()
{
    if (modulationViewActivated)
    {
        if (selectedView == "Q" || selectedView == "Feedback")
        {
            textModNotAvailable.setVisible(true);
        }
        else
        {
            textModNotAvailable.setVisible(false);
        }
    }
    else
    {
        textModNotAvailable.setVisible(false);
    }
}
