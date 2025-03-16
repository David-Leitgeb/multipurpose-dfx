/*
  ==============================================================================

    BandProcessor.h
    Created: 5 Apr 2024 3:06:10pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "juce_dsp/juce_dsp.h"
#include "FilterProcessor.h"
#include "DelayLineProcessor.h"
#include "PanoramaProcessor.h"
#include "GainProcessor.h"
#include "../ProcessorParameters.h"

class BandProcessor
{
public:
    BandProcessor(int givenBandID);

    void prepare(juce::dsp::ProcessSpec& spec, ProcessorParameters& parameterSettings);

    juce::dsp::AudioBlock<float> processBlock(juce::dsp::ProcessContextNonReplacing<float>&, ProcessorParameters&, std::vector<juce::dsp::AudioBlock<float>>&);

    int getBandID();

private:

    int bandID;

    // This method determines the correct modulation interval set in the GUI.
    int getModulationInterval(int selectedModulationInterval);

    // Instances of the processor classes which do all the signal processing.
    FilterProcessor filterProcessor;
    DelayLineProcessor delayLineProcessor;
    PanoramaProcessor panoramaProcessor;
    GainProcessor gainProcessor;
};