/*
  ==============================================================================

    BandComponentManager.h
    Created: 31 Dec 2023 12:36:38pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BandComponent.h"
#include "../LNF/TextButtonLNF.h"
#include "../LNF/ComboBoxLNF.h"
#include "../LNF/ToggleButtonLNF.h"

//==============================================================================
/*
*/
class BandComponentManager  : public juce::Component, public juce::Value::Listener, public juce::Button::Listener
{
public:
    BandComponentManager(juce::AudioProcessorValueTreeState&);
    ~BandComponentManager() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void valueChanged(juce::Value&) override;
	void buttonClicked(juce::Button*) override;

private:

    // Individual band component instances.
    BandComponent band1Component,
                  band2Component,
                  band3Component,
                  band4Component,
                  band5Component,
                  band6Component,
                  band7Component,
                  band8Component,
                  band9Component,
                  band10Component,
                  band11Component,
                  band12Component,
                  band13Component,
                  band14Component,
                  band15Component,
                  band16Component,
                  band17Component,
                  band18Component,
                  band19Component,
                  band20Component,
                  band21Component,
                  band22Component,
                  band23Component,
                  band24Component,
                  band25Component,
                  band26Component,
                  band27Component,
                  band28Component,
                  band29Component,
                  band30Component;

    // Components
    TextButtonLNF viewCutoffFreq{ "Cutoff" }, viewFilterQ{ "Q" }, viewDelayTime{ "Delay" }, viewFeedback{ "Feedback" }, viewPanorama{ "Pan" }, viewBandGain{ "Gain" };

    TextButtonLNF modulationViewLFO1{"LFO1"}, modulationViewLFO2{ "LFO2" }, modulationViewLFO3{ "LFO3" };

	ToggleButtonLNF viewModulation;

    ComboBoxLNF comboBoxModulationUpdateInterval;

    ComponentLabel modulationViewLabel{ "MODULATION:" }, modulationUpdateLabel{ "MOD UPDATE:" };

    // This object mantains connection between GUI components and APVTS parameters.
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modulationUpdateIntervalAttachment;

    // GUI view parameters
    bool modulationViewActivated = false;
    juce::String selectedView = "Cutoff";
    int selectedLFO = 1;
    juce::Value selectedBandValue{1};

    // Methods returning a vector containing pointers to component
    // instances for easier manipulation in the code.
	std::vector<BandComponent*> getBandComponents();

    std::vector<juce::Button*> getViewButtons();

    std::vector<juce::Button*> getModulationButtons();

    // Tooltip window to show description of given component.
    juce::TooltipWindow tooltipWindow;

    juce::DrawableText textModNotAvailable;

    void showModulationText();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandComponentManager)
};