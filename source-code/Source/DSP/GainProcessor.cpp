/*
  ==============================================================================

	GainProcessor.cpp
	Created: 25 Mar 2024 7:02:28pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "GainProcessor.h"

GainProcessor::GainProcessor() : sampleRate(44100.0f), gain(0.0f), lfoDepths({ &defaultLFODepth, &defaultLFODepth })
{

}

void GainProcessor::prepare(juce::dsp::ProcessSpec& spec)
{
	sampleRate = spec.sampleRate;
	gain.reset(sampleRate, 0.02f);
	lfoModSmoothed.reset(sampleRate, static_cast<float>(modulationInterval) / sampleRate);
}

void GainProcessor::processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals)
{

	for (int sample = 0; sample < block.getNumSamples(); ++sample)
	{

		for (int ch = 0; ch < block.getNumChannels(); ++ch)
		{
			float* data = block.getChannelPointer(ch);

			// LFO modulation
			float isModulated = 0;
			// Check if any of the LFOs is modulating the gain parameter
			for (int i = 0; i < lfoDepths.size(); ++i)
			{
				isModulated += *lfoDepths[i];
			}

			float gainModulated;
			// Gain is modulated:
			if (isModulated != 0.0f)
			{
				// The LFO modulation value is only calculated every specific number of samples (modulationInterval)
				// to improve performance
				if (modulationIndex >= modulationInterval)
				{
					lfoMod = 0.0f;
					// gain value is obtained from the GUI and modulated using the LFOs
					for (int i = 0; i < lfoSignals.size(); ++i)
					{
						float* lfo = lfoSignals[i].getChannelPointer(0);
						lfoMod += *lfoDepths[i] * (0.5f * (lfo[sample] + 1));
					}

					modulationIndex = 0;
				}
				else
				{
					modulationIndex++;
				}

				lfoModSmoothed.setTargetValue(lfoMod);
				gainModulated = juce::Decibels::decibelsToGain(gain.getNextValue()) + lfoModSmoothed.getNextValue();
			}
			// Gain is not modulated:
			else
			{
				// gain value obtained from the GUI
				gainModulated = juce::Decibels::decibelsToGain(gain.getNextValue());
			}

			data[sample] = gainModulated * data[sample];
		}
	}
}

void GainProcessor::setGain(float newGain)
{
	gain.setTargetValue(newGain);
}

void GainProcessor::setModulation(std::vector<float*>& newLfoDepths, int newModulationInterval)
{
	lfoDepths = newLfoDepths;
	modulationInterval = newModulationInterval;
}