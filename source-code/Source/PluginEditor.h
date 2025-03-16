/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/Components/HeaderComponent.h"
#include "GUI/Components/BandComponentManager.h"
#include "GUI/Components/LFOComponent.h"
#include "GUI/LNF/SliderLNF.h"

//==============================================================================
/**
*/
class MultiBandModulatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultiBandModulatorAudioProcessorEditor (MultiBandModulatorAudioProcessor&);
    ~MultiBandModulatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // Method used to make all the GUI components visible with
    // just one call in the constructor.
    void createGUI();

    // This reference is provided as a quick way for the editor to
    // access the processor object that created it.
    MultiBandModulatorAudioProcessor& audioProcessor;

    // GUI components
    HeaderComponent headerComponent;

    SliderLNF sliderInputGain,
        sliderOutputGain,
        sliderPluginMix,
        sliderGlobalFeedforward;

    ComponentLabel labelInputGain{ "In" }, labelOutputGain{ "Out" }, labelPluginMix{"Mix"}, labelGlobalFeedforward{"Feedforward"};

    BandComponentManager bandComponentManager;

    LFOComponent LFO1Component, LFO2Component, LFO3Component;

    // These objects mantain connection between GUI components and APVTS parameters.
    juce::AudioProcessorValueTreeState::SliderAttachment globalInputGainAttachment,
        globalOutputGainAttachment,
        globalMixAttachment,
        globalFeedforwardAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiBandModulatorAudioProcessorEditor)
};