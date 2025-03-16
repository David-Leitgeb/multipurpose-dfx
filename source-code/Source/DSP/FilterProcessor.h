/*
  ==============================================================================

    FilterProcessor.h
    Created: 25 Mar 2024 7:01:25pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "juce_dsp/juce_dsp.h"

class FilterProcessor
{
public:
    FilterProcessor();

    void prepare(juce::dsp::ProcessSpec& spec);

    void processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals);

	void setParameters(int newFilterType, float newCutoff, float newQ, float newGain, int newFilterOrder, std::vector<float*>& newLFODepths, int newModulationUpdateInterval);

private:
    // ProcessorDuplicator is used to process both channels at once.
	juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;

    // Variables used for coefficient calculations.
	const float sqrt2 = juce::MathConstants<float>::sqrt2;
    float V0, sqrt2V0, K, K2;

    // Methods containing the coefficient calculations.
    void updateFilter();
	void updateHighpassFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);
    void updateLowShelvingFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);
    void updateBandpassFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);
    void updateBandrejectFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);
    void updatePeakFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);
    void updateHighShelvingFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);
    void updateLowpassFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&);

    float sampleRate;

    // Filter parameters
    float filterCutoff, filterQ, filterGain;
    int filterType, filterOrder;
    bool filterNeedsUpdate = true;
    float filterCutoffModulated = 22000.0f;

    // LFO parameters
    std::vector<float*> lfoDepths;
    int modulationInterval = 64;
    int modulationIndex = 0;
    float lfoMod = 0.0f;
};
