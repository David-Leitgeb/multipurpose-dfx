/*
  ==============================================================================

	FilterProcessor.cpp
	Created: 25 Mar 2024 7:01:45pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "FilterProcessor.h"

FilterProcessor::FilterProcessor() : sampleRate(44100.0f)
{

}

void FilterProcessor::prepare(juce::dsp::ProcessSpec& spec)
{
	sampleRate = spec.sampleRate;
	updateFilter();
	filter.prepare(spec);
}

void FilterProcessor::processBlock(juce::dsp::AudioBlock<float>& block, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals)
{
	float isModulated = 0;
	// Check if any of the LFOs is modulating the cutoff parameter
	for (int i = 0; i < lfoDepths.size(); ++i)
	{
		isModulated += *lfoDepths[i];
	}

	// Filter cutoff is modulated:
	if (isModulated != 0.0f)
	{
		// Processing the audio block in smaller sub-blocks
		for (int sample = 0; sample < block.getNumSamples(); sample += modulationInterval)
		{
			// Check if the modulation value has changed
			if (modulationIndex >= modulationInterval)
			{
				lfoMod = 0.0f;
				// filter cutoff value is obtained from the GUI and modulated using the LFOs
				for (int i = 0; i < lfoSignals.size(); ++i)
				{
					float* lfo = lfoSignals[i].getChannelPointer(0);
					lfoMod += *lfoDepths[i] * lfo[sample];
				}

				// Update filter coefficients based on the new modulation value
				filterNeedsUpdate = true;

				modulationIndex = 0;
			}
			else
			{
				modulationIndex += modulationInterval;
			}

			filterCutoffModulated = filterCutoff + lfoMod;

			// Making sure that the filter cutoff value will not get out of range.
			if (filterCutoffModulated < 10)
				filterCutoffModulated = 10;

			if (filterCutoffModulated > 22000)
				filterCutoffModulated = 22000;

			// Process a sub-block of audio
			juce::dsp::AudioBlock<float> subBlock = block.getSubBlock(sample, std::min(modulationInterval, static_cast<int>(block.getNumSamples()) - sample));


			// The filter coefficients are calculated only when one of its parameters is changed by the user to improve performance.
			if (filterNeedsUpdate)
			{
				updateFilter();
			}

			// Filtering the signal:
			juce::dsp::ProcessContextReplacing<float> context(subBlock);
			filter.process(context);
		}
	}
	// Filter cutoff is not modulated:
	else
	{
		// filter cutoff value obtained from the GUI
		filterCutoffModulated = filterCutoff;

		// The filter coefficients are calculated only when one of its parameters is changed by the user to improve performance.
		if (filterNeedsUpdate)
		{
			updateFilter();
		}

		// Filtering the signal:
		juce::dsp::ProcessContextReplacing<float> context(block);
		filter.process(context);
	}
}

void FilterProcessor::setParameters(int newFilterType, float newCutoff, float newQ, float newGain, int newFilterOrder, std::vector<float*>& newLFODepths, int newModulationUpdateInterval)
{
	// Checking if any of the parameters were changed in the GUI
	if (filterType != newFilterType || filterCutoff != newCutoff || filterQ != newQ || filterGain != newGain || filterOrder != newFilterOrder)
	{
		filterType = newFilterType;
		filterCutoff = newCutoff;
		filterQ = newQ;
		filterGain = newGain;
		filterOrder = newFilterOrder;

		filterNeedsUpdate = true;
	}

	lfoDepths = newLFODepths;
	modulationInterval = newModulationUpdateInterval;
}

void FilterProcessor::updateFilter()
{
	V0 = pow(10.0f, (filterGain / 20.0f));
	sqrt2V0 = std::sqrt(2 * V0);
	K = std::tan(juce::MathConstants<float>::pi * filterCutoffModulated / sampleRate);
	K2 = K * K;

	// Choosing the correct filter type coefficient calculation.
	switch (filterType)
	{
	case 1: updateHighpassFilter(filter); break;		// Highpass
	case 2: updateLowShelvingFilter(filter);  break; // Low-shelving
	case 3: updateBandpassFilter(filter); break;		// Bandpass
	case 4: updateBandrejectFilter(filter); break;	// Bandreject
	case 5: updatePeakFilter(filter);  break;		// Peak
	case 6: updateHighShelvingFilter(filter); break;	// High-shelving
	case 7: updateLowpassFilter(filter); break;		// Lowpass
	default: break;
	}
}
// Remaining methods update the filter coefficients with newly calculated ones.
void FilterProcessor::updateHighpassFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>& filter)
{
	float b0, b1, b2, a0, a1, a2;
	float den;
	if (filterOrder == 1)
	{
		den = 1 / (K + 1);

		b0 = 1 * den;
		b1 = -1 * den;
		b2 = 0;
		a0 = 1;
		a1 = (K - 1) * den;
		a2 = 0;
	}
	else
	{
		den = 1 / (K2 * filterQ + K + filterQ);

		b0 = filterQ * den;
		b1 = (-2 * filterQ) * den;
		b2 = filterQ * den;
		a0 = 1;
		a1 = (2 * filterQ * (K2 - 1)) * den;
		a2 = (K2 * filterQ - K + filterQ) * den;
	}

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}

void FilterProcessor::updateLowShelvingFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&)
{
	float b0, b1, b2, a0, a1, a2;

	float denBoost = 1 / (1 + sqrt2 * K + K2);
	float denCut = 1 / (V0 + sqrt2V0 * K + K2);

	if (filterGain >= 0)
	{
		b0 = (1 + sqrt2V0 * K + V0 * K2) * denBoost;
		b1 = (2 * (V0 * K2 - 1)) * denBoost;
		b2 = (1 - sqrt2V0 * K + V0 * K2) * denBoost;
		a0 = 1;
		a1 = (2 * (K2 - 1)) * denBoost;
		a2 = (1 - sqrt2 * K + K2) * denBoost;
	}
	else
	{
		b0 = (V0 * (1 + sqrt2 * K + K2)) * denCut;
		b1 = (2 * V0 * (K2 - 1)) * denCut;
		b2 = (V0 * (1 - sqrt2 * K + K2)) * denCut;
		a0 = 1;
		a1 = (2 * (K2 - V0)) * denCut;
		a2 = (V0 - sqrt2V0 * K + K2) * denCut;
	}

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}

void FilterProcessor::updateBandpassFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&)
{
	float den = 1 / (K2 * filterQ + K + filterQ);

	float b0 = K * den;
	float b1 = 0;
	float b2 = -K * den;
	float a0 = 1;
	float a1 = (2 * filterQ * (K2 - 1)) * den;
	float a2 = (K2 * filterQ - K + filterQ) * den;

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}

void FilterProcessor::updateBandrejectFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&)
{
	float den = 1 / (K2 * filterQ + K + filterQ);

	float b0 = (filterQ * (1 + K2)) * den;
	float b1 = (2 * filterQ * (K2 - 1)) * den;
	float b2 = (filterQ * (1 + K2)) * den;
	float a0 = 1;
	float a1 = (2 * filterQ * (K2 - 1)) * den;
	float a2 = (K2 * filterQ - K + filterQ) * den;

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}

void FilterProcessor::updatePeakFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&)
{
	float b0, b1, b2, a0, a1, a2;
	float denBoost = 1 / ((1 + (1 / filterQ) * K + K2));
	float denCut = 1 / ((1 + (1 / (V0 * filterQ)) * K + K2));

	if (filterGain >= 0)
	{
		b0 = (1 + (V0 / filterQ) * K + K2) * denBoost;
		b1 = (2 * (K2 - 1)) * denBoost;
		b2 = (1 - (V0 / filterQ) * K + K2) * denBoost;
		a0 = 1;
		a1 = (2 * (K2 - 1)) * denBoost;
		a2 = (1 - (1 / filterQ) * K + K2) * denBoost;
	}
	else
	{
		b0 = (1 + (1 / filterQ) * K + K2) * denCut;
		b1 = (2 * (K2 - 1)) * denCut;
		b2 = (1 - (1 / filterQ) * K + K2) * denCut;
		a0 = 1;
		a1 = (2 * (K2 - 1)) * denCut;
		a2 = (1 - (1 / (V0 * filterQ)) * K + K2) * denCut;
	}

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}

void FilterProcessor::updateHighShelvingFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&)
{
	float b0, b1, b2, a0, a1, a2;
	float denBoost = 1 / (1 + sqrt2 * K + K2);
	float denCut = 1 / (1 + sqrt2V0 * K + V0 * K2);

	if (filterGain >= 0)
	{
		b0 = (V0 + sqrt2V0 * K + K2) * denBoost;
		b1 = (2 * (K2 - V0)) * denBoost;
		b2 = (V0 - sqrt2V0 * K + K2) * denBoost;
		a0 = 1;
		a1 = (2 * (K2 - 1)) * denBoost;
		a2 = (1 - sqrt2 * K + K2) * denBoost;
	}
	else
	{
		b0 = (V0 * (1 + sqrt2 * K + K2)) * denCut;
		b1 = (2 * V0 * (K2 - 1)) * denCut;
		b2 = (V0 * (1 - sqrt2 * K + K2)) * denCut;
		a0 = 1;
		a1 = (2 * (V0 * K2 - 1)) * denCut;
		a2 = (1 - sqrt2V0 * K + V0 * K2) * denCut;
	}

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}

void FilterProcessor::updateLowpassFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>& filter)
{
	float b0, b1, b2, a0, a1, a2;
	float den;
	if (filterOrder == 1)
	{
		den = 1 / (K + 1);

		b0 = K * den;
		b1 = K * den;
		b2 = 0;
		a0 = 1;
		a1 = (K - 1) * den;
		a2 = 0;
	}
	else
	{
		den = 1 / (K2 * filterQ + K + filterQ);

		b0 = (K2 * filterQ) * den;
		b1 = (2 * K2 * filterQ) * den;
		b2 = (K2 * filterQ) * den;
		a0 = 1;
		a1 = (2 * filterQ * (K2 - 1)) * den;
		a2 = (K2 * filterQ - K + filterQ) * den;
	}

	// Replacing the coefficients
	*filter.state = juce::dsp::IIR::Coefficients<float>::Coefficients(b0, b1, b2, a0, a1, a2);
	filterNeedsUpdate = false;
}
