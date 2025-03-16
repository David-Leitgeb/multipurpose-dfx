/*
  ==============================================================================

	PanoramaProcessor.cpp
	Created: 17 Mar 2024 1:24:31pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "PanoramaProcessor.h"

PanoramaProcessor::PanoramaProcessor() : sampleRate(44100.0f), panorama(0.0f)
{

}

void PanoramaProcessor::prepare(juce::dsp::ProcessSpec& spec)
{
	sampleRate = spec.sampleRate;
	panorama.reset(sampleRate, 0.02f);
	lfoModSmoothed.reset(sampleRate, static_cast<float>(modulationInterval) / sampleRate);
}

void PanoramaProcessor::processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals)
{
	for (int sample = 0; sample < block.getNumSamples(); ++sample)
	{
		for (int ch = 0; ch < block.getNumChannels(); ++ch)
		{
			float* data = block.getChannelPointer(ch);

			// LFO modulation
			float isModulated = 0;
			// Check if any of the LFOs is modulating the panorama parameter
			for (int i = 0; i < lfoDepths.size(); ++i)
			{
				isModulated += *lfoDepths[i];
			}

			float panoramaModulated;
			// Panorama is modulated:
			if (isModulated != 0.0f)
			{
				// The LFO modulation value is only calculated every specific number of samples (modulationInterval)
				// to improve performance.
				if (modulationIndex >= modulationInterval)
				{
					lfoMod = 0.0f;
					// panorama value is obtained from the GUI and modulated using the LFOs
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
				panoramaModulated = panorama.getNextValue() + 50 * lfoModSmoothed.getNextValue();
			}
			// Panorama is not modulated:
			else
			{
				// panorama value obtained from the GUI
				panoramaModulated = panorama.getNextValue();
			}

			// Making sure that the panorama value will not get out of range.
			if (panoramaModulated < -50)
				panoramaModulated = -50;
			if (panoramaModulated > 50)
				panoramaModulated = 50;

			// Range 0–1
			panoramaModulated = ((panoramaModulated / 50.0f) + 1) / 2.0f;
			// Left channel
			if (ch == 0)
			{
				data[sample] = data[sample] * (1.0f - panoramaModulated);
			}
			// Right channel
			else
			{
				data[sample] = data[sample] * panoramaModulated;
			}
		}
	}
}

void PanoramaProcessor::setParameters(int newPanorama, std::vector<float*>& newLFODepths, int newModulationUpdateInterval)
{
	panorama.setTargetValue(newPanorama);
	lfoDepths = newLFODepths;
	modulationInterval = newModulationUpdateInterval;
}
