/*
  ==============================================================================

    PanoramaProcessor.h
    Created: 17 Mar 2024 1:24:23pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "juce_dsp/juce_dsp.h"

class PanoramaProcessor
{
public:

	PanoramaProcessor();

    void prepare(juce::dsp::ProcessSpec& spec);

    void processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals);

    void setParameters(int newPanorama, std::vector<float*>& newLFODepths, int newModulationUpdateInterval);

private:
    float sampleRate;
    juce::SmoothedValue<int> panorama;

	std::vector<float*> lfoDepths;
    int modulationInterval = 64;
    int modulationIndex = 0;
    juce::SmoothedValue<float> lfoModSmoothed = 0.0f;
    float lfoMod = 0.0f;
};
