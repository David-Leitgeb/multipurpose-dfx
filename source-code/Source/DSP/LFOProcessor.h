/*
  ==============================================================================

    LFOProcessor.h
    Created: 8 May 2024 10:34:24am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "juce_dsp/juce_dsp.h"

class LFOProcessor
{
public:
    LFOProcessor();

    void prepare(juce::dsp::ProcessSpec& spec, int samplesPerBlock);

    juce::dsp::AudioBlock<float>& generateSignal();

    void setParameters(int newLfoShape, int newLfoRateSync, float newLfoRateHz, int newLfoDepth, bool newLfoRateMode, float newBpm, bool newHostIsPlaying, float newHostLfoPhase, bool newRetriggerLfo);

private:

    int sampleRate = 44100;

    // User defined parameters
    int lfoShape = 1, lfoRateSync = 3;
    juce::SmoothedValue<float> lfoDepth = 100;
    float lfoRateHz = 10.0f;
    bool lfoRateMode = false;

    // BPM and playback synchronization
    float bpm = 140;
    bool hostIsPlaying = false;
    float hostLfoPhase = 0;
    bool retriggerLFO = false;

    // Internal LFO variables
    float lfoPhase = 0;
    float noiseValue = 0;
    int noiseIndex = 0;
    juce::Random randNumGenerator;

    // Smoothing filter
    float lfoFilterCutoffFreq = 500;
    juce::dsp::IIR::Filter<float> lfoFilter;

    // LFO buffer
    juce::AudioBuffer<float> lfoBuffer;
    juce::dsp::AudioBlock<float> lfoBlock;
};