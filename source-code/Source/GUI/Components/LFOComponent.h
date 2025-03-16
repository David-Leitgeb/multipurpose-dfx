/*
  ==============================================================================

    LFOComponent.h
    Created: 5 Mar 2024 11:25:57am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ThemeColorManager.h"
#include "../LNF/SliderLNF.h"
#include "ComponentLabel.h"
#include "../LNF/ComboBoxLNF.h"
#include "../LNF/ToggleButtonLNF.h"

//==============================================================================
/*
*/
class LFOComponent  : public juce::Component
{
public:
    LFOComponent(juce::AudioProcessorValueTreeState&, int lfoIndex);
    ~LFOComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // Components
    ComboBoxLNF comboBoxLfoShape, comboBoxLfoRate;

    SliderLNF sliderLfoRate, sliderLfoDepth;

    ToggleButtonLNF buttonRateMode;

    ComponentLabel labelShape{"Shape"},
		labelDepth{ "Depth" },
        labelRateHz{ "Rate" },
        labelRateSync{ "Rate" },
        labelSyncMode{ "Sync" };

    juce::String lfoID;

    // These objects mantain connection between GUI components and APVTS parameters.
    juce::AudioProcessorValueTreeState::SliderAttachment lfoRateAttachment, lfoDepthAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment lfoRateModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfoShapeAttachment, lfoRateSyncAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
