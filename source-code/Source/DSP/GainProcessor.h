/*
  ==============================================================================

    GainProcessor.h
    Created: 25 Mar 2024 7:02:28pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "juce_dsp/juce_dsp.h"

class GainProcessor
{
public:

    GainProcessor();

    void prepare(juce::dsp::ProcessSpec& spec);

    void processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals);

    void setGain(float newGain);

    void setModulation(std::vector<float*>& newLfoDepths, int newModulationInterval);

private:
    float sampleRate;
    juce::SmoothedValue<float> gain;
    std::vector<float*> lfoDepths;
    float defaultLFODepth = 0;
    // LFO parameters
    int modulationInterval = 64;
    int modulationIndex = 0;
    juce::SmoothedValue<float> lfoModSmoothed = 0.0f;
    float lfoMod = 0.0f;
};
