/*
  ==============================================================================

	BandComponent.cpp
	Created: 30 Dec 2023 12:57:31pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BandComponent.h"

#include "juce_core/unit_tests/juce_UnitTestCategories.h"

//==============================================================================
BandComponent::BandComponent(juce::AudioProcessorValueTreeState& apvts, int bandIndex, juce::Value& selectedBandIndex) :
	// APVTS attachments
	filterCuttoffAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterCutoff", sliderFilterCutoff),
	filterQAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterQ", sliderFilterQ),
	filterGainAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterGain", sliderFilterGain),
	delayAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Delay", sliderDelay),
	feedbackAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Feedback", sliderFeedback),
	feedforwardAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Feedforward", sliderFeedforward),
	blendAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Blend", sliderBlend),
	panoramaAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Panorama", sliderPan),
	gainAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Gain", sliderGain),
	filterCuttoffVerticalAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterCutoff", sliderVerticalFilterCutoff),
	filterQVerticalAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterQ", sliderVerticalFilterQ),
	panoramaVerticalAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Panorama", sliderVerticalPanorama),
	delayTimeVerticalAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Delay", sliderVerticalDelayTime),
	feedbackVerticalAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Feedback", sliderVerticalFeedback),
	gainVerticalAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Gain", sliderVerticalGain),
	// LFO modulation
	lfo1DepthGainAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO1DepthGain", sliderLFO1DepthGain),
	lfo2DepthGainAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO2DepthGain", sliderLFO2DepthGain),
	lfo3DepthGainAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO3DepthGain", sliderLFO3DepthGain),
	lfo1DepthDelayAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO1DepthDelay", sliderLFO1DepthDelay),
	lfo2DepthDelayAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO2DepthDelay", sliderLFO2DepthDelay),
	lfo3DepthDelayAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO3DepthDelay", sliderLFO3DepthDelay),
	lfo1DepthPanAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO1DepthPan", sliderLFO1DepthPan),
	lfo2DepthPanAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO2DepthPan", sliderLFO2DepthPan),
	lfo3DepthPanAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO3DepthPan", sliderLFO3DepthPan),
	lfo1DepthCutoffAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO1DepthCutoff", sliderLFO1DepthCutoff),
	lfo2DepthCutoffAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO2DepthCutoff", sliderLFO2DepthCutoff),
	lfo3DepthCutoffAttachment(apvts, "Band" + static_cast<juce::String>(bandIndex) + "LFO3DepthCutoff", sliderLFO3DepthCutoff),
	buttonSelectBand{ static_cast<juce::String>(bandIndex) },
	selectedBandID(selectedBandIndex)
{
	bandID = bandIndex;

	// Combo box settings
	comboBoxFilterType.addItem("Highpass", 1);
	comboBoxFilterType.addItem("Low-shelving", 2);
	comboBoxFilterType.addItem("Bandpass", 3);
	comboBoxFilterType.addItem("Bandreject", 4);
	comboBoxFilterType.addItem("Peak", 5);
	comboBoxFilterType.addItem("High-shelving", 6);
	comboBoxFilterType.addItem("Lowpass", 7);
	comboBoxFilterType.addItem("Off", 8);
	comboBoxFilterType.setSelectedId(7);

	comboBoxFilterOrder.addItem("1", 1);
	comboBoxFilterOrder.addItem("2", 2);
	comboBoxFilterOrder.setSelectedId(2);


	// Combo box attachments have to be constructed after the items have been added.
	filterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterType", comboBoxFilterType);
	filterOrderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "Band" + static_cast<juce::String>(bandIndex) + "FilterOrder", comboBoxFilterOrder);

	// Adding the band controls.
	for (auto* component : getBandControls())
	{
		addChildComponent(component);
	}

	// Setting up the solo button behaviour.
	buttonSoloBand.setClickingTogglesState(true);
	buttonSoloBand.setColour(juce::TextButton::textColourOnId, ThemeColorManager::mdmTrioColor);

	// Setting the correct slider suffix
	sliderFilterCutoff.setTextValueSuffix(" Hz");
	sliderFilterGain.setTextValueSuffix(" dB");
	sliderGain.setTextValueSuffix(" dB");
	sliderDelay.setTextValueSuffix(" ms");
	sliderFeedback.setTextValueSuffix(" %");
	sliderFeedforward.setTextValueSuffix(" %");

	// Vertical section
	for (auto* component : getVerticalControls())
	{
		addAndMakeVisible(component);
		if (component == &sliderVerticalFilterQ || component == &sliderVerticalPanorama || component == &sliderVerticalDelayTime || component == &sliderVerticalFeedback || component == &sliderVerticalGain)
		{
			component->setVisible(false);
		}
	}

	// Setting the vertical slider colour.
	sliderVerticalFilterCutoff.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewCutoff);
	sliderVerticalFilterQ.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewQ);
	sliderVerticalDelayTime.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewDelay);
	sliderVerticalFeedback.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewFeedback);
	sliderVerticalPanorama.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewPanorama);
	sliderVerticalGain.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewGain);

	// Adding listeners that handle correct GUI behaviour
	buttonSelectBand.addListener(this);
	sliderVerticalFilterCutoff.addListener(this);
	sliderVerticalFilterQ.addListener(this);
	sliderVerticalDelayTime.addListener(this);
	sliderVerticalFeedback.addListener(this);
	sliderVerticalPanorama.addListener(this);
	sliderVerticalGain.addListener(this);

	// Setting up the solo button parameters.
	buttonBypassBand.setColour(juce::TextButton::textColourOnId, ThemeColorManager::mdmTrioColor);
	buttonBypassBand.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	// Solo and Bypass APVTS attachments.
	bandBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Bypass", buttonBypassBand);
	bandSoloAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "Band" + static_cast<juce::String>(bandIndex) + "Solo", buttonSoloBand);

	// Selecting the first band when the plugin is first opened.
	if (bandID == 1)
	{
		buttonSelectBand.setColour(juce::TextButton::textColourOffId, ThemeColorManager::mdmTrioColor);
		buttonBypassBand.setToggleState(false, juce::NotificationType::sendNotification);

		for (auto* component : getBandControls())
		{
			component->setVisible(true);
		}

		// Label is only shown in the first instance to avoid layering them multiple times.
		// Setting the components visible.
		addAndMakeVisible(labelFilterType);
		addAndMakeVisible(labelFilterCutoff);
		addAndMakeVisible(labelFilterQ);
		addAndMakeVisible(labelFilterGain);
		addAndMakeVisible(labelFilterOrder);
		addAndMakeVisible(labelPan);
		addAndMakeVisible(labelGain);
		addAndMakeVisible(labelDelay);
		addAndMakeVisible(labelFeedback);
		addAndMakeVisible(labelFeedforward);
		addAndMakeVisible(labelBlend);
	}
	// Setting the components visible
	addChildComponent(sliderLFO1DepthGain);
	addChildComponent(sliderLFO2DepthGain);
	addChildComponent(sliderLFO3DepthGain);
	addChildComponent(sliderLFO1DepthDelay);
	addChildComponent(sliderLFO2DepthDelay);
	addChildComponent(sliderLFO3DepthDelay);
	addChildComponent(sliderLFO1DepthPan);
	addChildComponent(sliderLFO2DepthPan);
	addChildComponent(sliderLFO3DepthPan);
	addChildComponent(sliderLFO1DepthCutoff);
	addChildComponent(sliderLFO2DepthCutoff);
	addChildComponent(sliderLFO3DepthCutoff);

	// Adjusting the slider colours.
	sliderFilterCutoff.setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmViewCutoff);
	sliderLFO1DepthCutoff.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewCutoff.darker(0.6f));
	sliderLFO2DepthCutoff.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewCutoff.darker(0.6f));
	sliderLFO3DepthCutoff.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewCutoff.darker(0.6f));
	sliderFilterQ.setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmViewQ);
	sliderDelay.setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmViewDelay);
	sliderLFO1DepthDelay.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewDelay.darker(0.6f));
	sliderLFO2DepthDelay.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewDelay.darker(0.6f));
	sliderLFO3DepthDelay.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewDelay.darker(0.6f));
	sliderFeedback.setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmViewFeedback);
	sliderPan.setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmViewPanorama);
	sliderLFO1DepthPan.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewPanorama.darker(0.6f));
	sliderLFO2DepthPan.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewPanorama.darker(0.6f));
	sliderLFO3DepthPan.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewPanorama.darker(0.6f));
	sliderGain.setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmViewGain);
	sliderLFO1DepthGain.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewGain.darker(0.6f));
	sliderLFO2DepthGain.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewGain.darker(0.6f));
	sliderLFO3DepthGain.setColour(juce::Slider::trackColourId, ThemeColorManager::mdmViewGain.darker(0.6f));
}

BandComponent::~BandComponent()
{
}

void BandComponent::paint(juce::Graphics& g)
{
}

void BandComponent::resized()
{
	// Defining rectangles which are then used as bounds for individual components.
	const float cornerSize = getLocalBounds().getWidth() * 0.016f;
	juce::Rectangle<int> localBounds = getLocalBounds().withTrimmedLeft(cornerSize).withTrimmedRight(cornerSize);//.reduced(getHeight() / 10); // .withTrimmedTop(getHeight() / 10) groupborder

	const int localWidth = localBounds.getWidth();
	const float localHeight = localBounds.getHeight();
	constexpr int componentCount = 11;
	const int labelHeight = localBounds.getHeight() * 0.075f;

	juce::Rectangle<int> rectBandControls = localBounds.removeFromTop(localHeight * 0.3f),
		rectVerticalSection = localBounds;

	const float verticalSectionBandWidth = rectVerticalSection.getWidth() / numBands;

	juce::Rectangle<int> rectFilterType = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectFilterCutoff = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectFilterQ = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectFilterGain = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectFilterOrder = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectDelay = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectFeedback = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectFeedforward = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectBlend = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectPan = rectBandControls.removeFromLeft(localWidth / componentCount),
		rectGain = rectBandControls.removeFromLeft(localWidth / componentCount);

	// Setting the component bounds.
	labelFilterType.setBounds(rectFilterType.removeFromTop(labelHeight));
	comboBoxFilterType.setBounds(rectFilterType.withTrimmedTop(rectFilterType.getHeight() * 0.05f).withTrimmedBottom(rectFilterType.getHeight() * 0.65f));

	sliderFilterCutoff.setBounds(rectFilterCutoff);
	labelFilterCutoff.setBounds(rectFilterCutoff.removeFromTop(labelHeight));

	sliderFilterQ.setBounds(rectFilterQ);
	labelFilterQ.setBounds(rectFilterQ.removeFromTop(labelHeight));

	sliderFilterGain.setBounds(rectFilterGain);
	labelFilterGain.setBounds(rectFilterGain.removeFromTop(labelHeight));

	labelFilterOrder.setBounds(rectFilterOrder.removeFromTop(labelHeight));
	comboBoxFilterOrder.setBounds(rectFilterOrder.withTrimmedTop(rectFilterOrder.getHeight() * 0.05f).withTrimmedBottom(rectFilterOrder.getHeight() * 0.65f));

	sliderDelay.setBounds(rectDelay);
	labelDelay.setBounds(rectDelay.removeFromTop(labelHeight));

	sliderFeedback.setBounds(rectFeedback);
	labelFeedback.setBounds(rectFeedback.removeFromTop(labelHeight));

	sliderFeedforward.setBounds(rectFeedforward);
	labelFeedforward.setBounds(rectFeedforward.removeFromTop(labelHeight));

	sliderBlend.setBounds(rectBlend);
	labelBlend.setBounds(rectBlend.removeFromTop(labelHeight));

	sliderPan.setBounds(rectPan);
	labelPan.setBounds(rectPan.removeFromTop(labelHeight));

	sliderGain.setBounds(rectGain);
	labelGain.setBounds(rectGain.removeFromTop(labelHeight));

	// Determining the correct vertical components location depending on the index of the given band component instance.
	rectVerticalSection = rectVerticalSection.withTrimmedLeft(verticalSectionBandWidth * (bandID - 1)).removeFromLeft(verticalSectionBandWidth);

	float reduceSize = rectVerticalSection.getWidth() * 0.1f;
	// Bounds for the remaining components.
	auto rectBypassButton = rectVerticalSection.removeFromTop(rectVerticalSection.getHeight() * 0.05),
		rectSelectButton = rectVerticalSection.removeFromTop(rectVerticalSection.getWidth()).reduced(reduceSize),
		rectVerticalSlider = rectVerticalSection.removeFromTop(localBounds.getHeight() * 0.65).reduced(reduceSize),
		rectSoloButton = rectVerticalSection.removeFromTop(rectVerticalSection.getWidth()).reduced(reduceSize);


	// Setting the component bounds
	buttonBypassBand.setBounds(rectBypassButton);
	buttonSelectBand.setBounds(rectSelectButton);
	sliderVerticalFilterCutoff.setBounds(rectVerticalSlider);
	sliderVerticalFilterQ.setBounds(rectVerticalSlider);
	sliderVerticalPanorama.setBounds(rectVerticalSlider);
	sliderVerticalDelayTime.setBounds(rectVerticalSlider);
	sliderVerticalFeedback.setBounds(rectVerticalSlider);
	sliderVerticalGain.setBounds(rectVerticalSlider);

	buttonSoloBand.setBounds(rectSoloButton);

	juce::Rectangle<int> rectModulationSliders = rectVerticalSlider;

	sliderLFO1DepthGain.setBounds(rectModulationSliders);
	sliderLFO2DepthGain.setBounds(rectModulationSliders);
	sliderLFO3DepthGain.setBounds(rectModulationSliders);
	sliderLFO1DepthDelay.setBounds(rectModulationSliders);
	sliderLFO2DepthDelay.setBounds(rectModulationSliders);
	sliderLFO3DepthDelay.setBounds(rectModulationSliders);
	sliderLFO1DepthPan.setBounds(rectModulationSliders);
	sliderLFO2DepthPan.setBounds(rectModulationSliders);
	sliderLFO3DepthPan.setBounds(rectModulationSliders);
	sliderLFO1DepthCutoff.setBounds(rectModulationSliders);
	sliderLFO2DepthCutoff.setBounds(rectModulationSliders);
	sliderLFO3DepthCutoff.setBounds(rectModulationSliders);

}

// This method makes it possible to control all the components independently
// even though the bounds of their parents are overlapping.
bool BandComponent::hitTest(int x, int y)
{
	for (auto* child : getChildren())
	{
		if (child->isVisible() && child->getBounds().contains(x, y))
		{
			if (child->hitTest(x, y))
				return true;
		}
	}
	return false;
}

// Clicking the select button will change the ID of the selected band stored in the juce::Value
// variable reference from the parent class. Changing this value calls the valueChanged method
// from the parent class which then calls changeSelectedBand method located right under this
// one. This approach was used because the native JUCE radio button grouping only works between
// buttons located in the same parent component.
void BandComponent::buttonClicked(juce::Button* button)
{
	selectedBandID.setValue(bandID);
	repaint();
}

void BandComponent::sliderValueChanged(juce::Slider* slider)
{
	// Only executed when the band selection changes.
	if (static_cast<int>(selectedBandID.getValue()) != bandID)
	{
		selectedBandID.setValue(bandID);
		repaint();
	}
}


// This method manages the visibility of the band control components in the upper part of the band window section.
void BandComponent::changeSelectedBand()
{
	int selectedBand = selectedBandID.getValue();

	if (selectedBand == bandID)
	{
		// set visible
		for (auto* component : getBandControls())
		{
			component->setVisible(true);
		}

		buttonSelectBand.setColour(juce::TextButton::textColourOffId, ThemeColorManager::mdmTrioColor);
		repaint();
	}
	else
	{
		// set invisible
		for (auto* component : getBandControls())
		{
			component->setVisible(false);
		}

		buttonSelectBand.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
		repaint();
	}
}

// This method changes the visible components in the vertical section.
void BandComponent::changeVerticalSliderView(juce::String view, bool modulationViewActivated, int selectedLFO)
{
	if (view == "Cutoff")
	{
		sliderVerticalFilterCutoff.setVisible(!modulationViewActivated);
		sliderLFO1DepthCutoff.setVisible(modulationViewActivated && selectedLFO == 1);
		sliderLFO2DepthCutoff.setVisible(modulationViewActivated && selectedLFO == 2);
		sliderLFO3DepthCutoff.setVisible(modulationViewActivated && selectedLFO == 3);
	}
	else
	{
		sliderVerticalFilterCutoff.setVisible(false);
		sliderLFO1DepthCutoff.setVisible(false);
		sliderLFO2DepthCutoff.setVisible(false);
		sliderLFO3DepthCutoff.setVisible(false);
	}

	if (view == "Q" && !modulationViewActivated)
		sliderVerticalFilterQ.setVisible(true);
	else
		sliderVerticalFilterQ.setVisible(false);

	if (view == "Delay")
	{
		sliderVerticalDelayTime.setVisible(!modulationViewActivated);
		sliderLFO1DepthDelay.setVisible(modulationViewActivated && selectedLFO == 1);
		sliderLFO2DepthDelay.setVisible(modulationViewActivated && selectedLFO == 2);
		sliderLFO3DepthDelay.setVisible(modulationViewActivated && selectedLFO == 3);
	}
	else
	{
		sliderVerticalDelayTime.setVisible(false);
		sliderLFO1DepthDelay.setVisible(false);
		sliderLFO2DepthDelay.setVisible(false);
		sliderLFO3DepthDelay.setVisible(false);
	}

	if (view == "Feedback" && !modulationViewActivated)
		sliderVerticalFeedback.setVisible(!modulationViewActivated);
	else
		sliderVerticalFeedback.setVisible(false);

	if (view == "Pan")
	{
		sliderVerticalPanorama.setVisible(!modulationViewActivated);
		sliderLFO1DepthPan.setVisible(modulationViewActivated && selectedLFO == 1);
		sliderLFO2DepthPan.setVisible(modulationViewActivated && selectedLFO == 2);
		sliderLFO3DepthPan.setVisible(modulationViewActivated && selectedLFO == 3);
	}
	else
	{
		sliderVerticalPanorama.setVisible(false);
		sliderLFO1DepthPan.setVisible(false);
		sliderLFO2DepthPan.setVisible(false);
		sliderLFO3DepthPan.setVisible(false);
	}

	if (view == "Gain")
	{
		sliderVerticalGain.setVisible(!modulationViewActivated);
		sliderLFO1DepthGain.setVisible(modulationViewActivated && selectedLFO == 1);
		sliderLFO2DepthGain.setVisible(modulationViewActivated && selectedLFO == 2);
		sliderLFO3DepthGain.setVisible(modulationViewActivated && selectedLFO == 3);
	}
	else
	{
		sliderVerticalGain.setVisible(false);
		sliderLFO1DepthGain.setVisible(false);
		sliderLFO2DepthGain.setVisible(false);
		sliderLFO3DepthGain.setVisible(false);
	}
}

std::vector<juce::Component*> BandComponent::getBandControls()
{
	return
	{
		&comboBoxFilterType,
		&sliderFilterCutoff,
		&sliderFilterQ,
		&sliderFilterGain,
		&comboBoxFilterOrder,
		&sliderPan,
		&sliderGain,
		&sliderDelay,
		&sliderFeedback,
		&sliderFeedforward,
		&sliderBlend
	};
}

std::vector<juce::Component*> BandComponent::getVerticalControls()
{
	return
	{
		&buttonSelectBand,
		&buttonSoloBand,
		&buttonBypassBand,
		&sliderVerticalFilterCutoff,
		&sliderVerticalFilterQ,
		&sliderVerticalPanorama,
		&sliderVerticalDelayTime,
		&sliderVerticalFeedback,
		&sliderVerticalGain
	};
}
