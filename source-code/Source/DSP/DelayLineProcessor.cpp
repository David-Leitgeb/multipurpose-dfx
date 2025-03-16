/*
  ==============================================================================

	DelayLineProcessor.cpp
	Created: 25 Mar 2024 7:02:03pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "DelayLineProcessor.h"

DelayLineProcessor::DelayLineProcessor() : circularBuffer(2, 44100 * 2), sampleRate(44100.0f), delayTime(1.0f), feedforwardGain(100), feedbackGain(0), blendGain(0)
{
	circularBuffer.clear();
}

void DelayLineProcessor::prepare(juce::dsp::ProcessSpec& spec)
{
	sampleRate = spec.sampleRate;
	delayTime.reset(sampleRate, 0.02f);
	lfoModSmoothed.reset(sampleRate, static_cast<float>(modulationInterval) / sampleRate);
	// Making sure the circular buffer parameters are adjusted depending on the audio settings.
	circularBuffer.setSize(spec.numChannels, spec.sampleRate * maxDelayTime, true, true, false);
}

void DelayLineProcessor::processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals)
{
	int circularBufferNumSamples = circularBuffer.getNumSamples();

	for (int sample = 0; sample < block.getNumSamples(); ++sample)
	{
		for (int ch = 0; ch < block.getNumChannels(); ++ch)
		{
			float* data = block.getChannelPointer(ch);

			// LFO modulation
			float isModulated = 0;
			// Check if any of the LFOs is modulating the delay parameter
			for (int i = 0; i < lfoDepths.size(); ++i)
			{
				isModulated += *lfoDepths[i];
			}

			float delayTimeModulated;
			// Delay is modulated:
			if (isModulated != 0.0f)
			{
				// The LFO modulation value is only calculated every specific number of samples (modulationInterval)
				// to improve performance
				if (modulationIndex >= modulationInterval)
				{
					lfoMod = 0.0f;
					// delay value is obtained from the GUI and modulated using the LFOs
					for (int i = 0; i < lfoSignals.size(); ++i)
					{
						float* lfo = lfoSignals[i].getChannelPointer(0);
						lfoMod += *lfoDepths[i] * lfo[sample];
					}

					modulationIndex = 0;
				}
				else
				{
					modulationIndex++;
				}

				lfoModSmoothed.setTargetValue(lfoMod);
				delayTimeModulated = delayTime.getNextValue() + lfoModSmoothed.getNextValue();

				// Making sure that the delay value will not get out of range.
				if (delayTimeModulated < 1.0f)
				{
					delayTimeModulated = 1.0f;
				}
			}
			// Delay is not modulated:
			else
			{
				// delay value obtained from the GUI
				delayTimeModulated = delayTime.getNextValue();
			}

			// Read index
			float dpr = fmod(dpw - delayTimeModulated * std::pow(10, -3) * sampleRate + circularBufferNumSamples - 3, circularBufferNumSamples);;
			// Polynomial interpolation
			float delaySignal = polynomialInterpolation(dpr, ch);
			// Rest of the delay line calculations
			float inputSignal = data[sample];
			float xh = inputSignal + (feedbackGain / 100) * delaySignal;
			data[sample] = (feedforwardGain / 100) * delaySignal + (blendGain / 100) * xh;
			circularBuffer.setSample(ch, dpw, xh);
		}
		// Adjusting the write index
		dpw = dpw + 1;
		if (dpw >= circularBufferNumSamples)
			dpw = 0;
	}
}

void DelayLineProcessor::setParameters(float newDelayTime, float newFeedforwardGain, float newFeedbackGain, float newBlendGain, std::vector<float*>& newLFODepths, int newModulationUpdateInterval)
{
	delayTime.setTargetValue(newDelayTime);
	feedforwardGain = newFeedforwardGain;
	feedbackGain = newFeedbackGain;
	blendGain = newBlendGain;
	lfoDepths = newLFODepths;
	modulationInterval = newModulationUpdateInterval;
}

// This method performs polynomial interpolation of samples.
float DelayLineProcessor::polynomialInterpolation(float dpr, int ch)
{
	int circularBufferNumSamples = circularBuffer.getNumSamples();

	// Sample index calculations
	int currentSampleIndex = static_cast<int>(floorf(dpr)) % circularBufferNumSamples;
	int previousSampleIndex = (currentSampleIndex - 1 + circularBufferNumSamples) % circularBufferNumSamples;
	int nextSampleIndex = (currentSampleIndex + 1) % circularBufferNumSamples;

	// Samples are stored in these variables for better readability of the following calculations
	float currentSample = circularBuffer.getSample(ch, currentSampleIndex);
	float previousSample = circularBuffer.getSample(ch, previousSampleIndex);
	float nextSample = circularBuffer.getSample(ch, nextSampleIndex);

	// Coefficients
	float c0 = currentSample;
	float c1 = 0.5f * (nextSample - previousSample);
	float c2 = 0.5f * (nextSample - 2 * currentSample + previousSample);

	float tau = dpr - floorf(dpr);

	// Interpolated signal from the delay line
	return c2 * (tau * tau) + c1 * tau + c0;
}
// This method performs linear interpolation of samples.
float DelayLineProcessor::linearInterpolation(float dpr, int ch)
{
	int circularBufferNumSamples = circularBuffer.getNumSamples();

	// Sample index calculations
	float fraction = dpr - floorf(dpr);
	int previousSampleIndex = static_cast<int>(floorf(dpr)) % circularBufferNumSamples;
	int nextSampleIndex = (previousSampleIndex + 1) % circularBufferNumSamples;

	// Interpolated signal from the delay line
	return fraction * circularBuffer.getSample(ch, nextSampleIndex) + (1 - fraction) * circularBuffer.getSample(ch, previousSampleIndex);
}