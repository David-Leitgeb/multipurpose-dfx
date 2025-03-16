/*
  ==============================================================================

    DelayLineProcessor.h
    Created: 25 Mar 2024 7:02:03pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "juce_dsp/juce_dsp.h"

class DelayLineProcessor
{
public:
    DelayLineProcessor();

    void prepare(juce::dsp::ProcessSpec& spec);

    void processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals);

    void setParameters(float newDelayTime, float newFeedforwardGain, float newFeedbackGain, float newBlendGain, std::vector<float*>& newLFODepths, int newModulationUpdateInterval);

private:
    // Internal circular buffer
    juce::AudioBuffer<float> circularBuffer;
    // Write index
    int dpw = 0;

    float maxDelayTime = 2;
    float sampleRate, feedforwardGain, feedbackGain, blendGain;
    juce::SmoothedValue<float> delayTime;

    // LFO parameeters
    int modulationInterval = 64;
    int modulationIndex = 0;
    juce::SmoothedValue<float> lfoModSmoothed = 0.0f;
    float lfoMod = 0.0f;
    std::vector<float*> lfoDepths;

    // Each of these methods provides a different type of sample interpolation.
	float polynomialInterpolation(float dpr, int ch);
    float linearInterpolation(float dpr, int ch);
};
