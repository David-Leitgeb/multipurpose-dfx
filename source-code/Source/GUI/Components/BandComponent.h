/*
  ==============================================================================

	BandComponent.h
	Created: 30 Dec 2023 12:57:31pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ComponentLabel.h"
#include "../LNF/SliderLNF.h"
#include "../LNF/TextButtonLNF.h"
#include "../LNF/VerticalSliderLNF.h"
#include "../LNF/ComboBoxLNF.h"
#include "../LNF/ToggleButtonLNF.h"

//==============================================================================
/*
*/
class BandComponent : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener
{
public:
	BandComponent(juce::AudioProcessorValueTreeState&, int, juce::Value&);
	~BandComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	bool hitTest(int, int) override;

	void buttonClicked(juce::Button*) override;
	void sliderValueChanged(juce::Slider* slider) override;

	void changeSelectedBand();
	void changeVerticalSliderView(juce::String, bool, int);

private:

	int numBands = 30;
	int bandID;
	int numLFOs = 3;

	juce::Value& selectedBandID;

	// Components
	SliderLNF sliderFilterCutoff,
		sliderFilterQ,
		sliderFilterGain,
		sliderDelay,
		sliderFeedback,
		sliderFeedforward,
		sliderBlend,
		sliderPan,
		sliderGain;

	VerticalSliderLNF sliderVerticalFilterCutoff,
		sliderVerticalFilterQ,
		sliderVerticalPanorama,
		sliderVerticalDelayTime,
		sliderVerticalFeedback,
		sliderVerticalGain;

	VerticalSliderLNF sliderLFO1DepthGain,
		sliderLFO2DepthGain,
		sliderLFO3DepthGain,

		sliderLFO1DepthDelay,
		sliderLFO2DepthDelay,
		sliderLFO3DepthDelay,

		sliderLFO1DepthPan,
		sliderLFO2DepthPan,
		sliderLFO3DepthPan,

		sliderLFO1DepthCutoff,
		sliderLFO2DepthCutoff,
		sliderLFO3DepthCutoff;

	ComboBoxLNF comboBoxFilterType, comboBoxFilterOrder;

	TextButtonLNF buttonSelectBand, buttonSoloBand{ "S" }; //, buttonBypassBand{ "B" };

	ToggleButtonLNF buttonBypassBand;

	ComponentLabel labelFilterType{ "Type" },
		labelFilterCutoff{ "Cutoff" },
		labelFilterQ{ "Q" },
		labelFilterGain{ "Gain" },
		labelFilterOrder{ "Order" },
		labelDelay{ "Delay" },
		labelFeedback{ "Feedback" },
		labelFeedforward{ "Feedforward" },
		labelBlend{ "Blend" },
		labelPan{ "Pan" },
		labelGain{ "Gain" };

	// These objects mantain connection between GUI components and APVTS parameters.
	juce::AudioProcessorValueTreeState::SliderAttachment lfo1DepthGainAttachment, lfo2DepthGainAttachment, lfo3DepthGainAttachment,
		lfo1DepthDelayAttachment, lfo2DepthDelayAttachment, lfo3DepthDelayAttachment,
		lfo1DepthPanAttachment, lfo2DepthPanAttachment, lfo3DepthPanAttachment,
		lfo1DepthCutoffAttachment, lfo2DepthCutoffAttachment, lfo3DepthCutoffAttachment;

	juce::AudioProcessorValueTreeState::SliderAttachment filterCuttoffAttachment, filterQAttachment, filterGainAttachment,
		delayAttachment, feedbackAttachment, feedforwardAttachment, blendAttachment, panoramaAttachment, gainAttachment,
		filterCuttoffVerticalAttachment, filterQVerticalAttachment, panoramaVerticalAttachment, delayTimeVerticalAttachment, // VERTICAL SLIDERS
		feedbackVerticalAttachment, gainVerticalAttachment;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bandBypassAttachment, bandSoloAttachment;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment, filterOrderAttachment;

	// Methods returning a vector containing pointers to component
	// instances for easier manipulation in the code.
	std::vector<juce::Component*> getBandControls();
	std::vector<juce::Component*> getVerticalControls();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandComponent)
};