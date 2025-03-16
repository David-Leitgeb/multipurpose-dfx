/*
  ==============================================================================

	BandProcessor.cpp
	Created: 5 Apr 2024 3:06:10pm
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "BandProcessor.h"

BandProcessor::BandProcessor(int givenBandID)
{
	// -1 to compensate zero based vector indexing.
	bandID = givenBandID - 1;
}

void BandProcessor::prepare(juce::dsp::ProcessSpec& spec, ProcessorParameters& parameterSettings)
{
	// The following vectors store pointers to the LFO depths for all parameters that can be modulated.
	std::vector<float*>lfoGainDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthGain,
	&parameterSettings.bandParameters[bandID].lfo2DepthGain, &parameterSettings.bandParameters[bandID].lfo3DepthGain };

	std::vector<float*>lfoDelayDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthDelay,
	&parameterSettings.bandParameters[bandID].lfo2DepthDelay, &parameterSettings.bandParameters[bandID].lfo3DepthDelay };

	std::vector<float*>lfoPanDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthPan,
	&parameterSettings.bandParameters[bandID].lfo2DepthPan,&parameterSettings.bandParameters[bandID].lfo3DepthPan };

	std::vector<float*>lfoCutoffDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthCutoff,
	&parameterSettings.bandParameters[bandID].lfo2DepthCutoff, &parameterSettings.bandParameters[bandID].lfo3DepthCutoff };


	// Obtaining the correct modulation interval set in the GUI.
	int modulationInterval = getModulationInterval(parameterSettings.modulationUpdateInterval);

	// FILTER
	// If the filter type is set to none, filter processor methods will not be called at all.
	if (parameterSettings.bandParameters[bandID].bandFilterType != 8)
	{
		filterProcessor.setParameters(parameterSettings.bandParameters[bandID].bandFilterType, parameterSettings.bandParameters[bandID].bandFilterCutoff, parameterSettings.bandParameters[bandID].bandFilterQ, parameterSettings.bandParameters[bandID].bandFilterGain, parameterSettings.bandParameters[bandID].bandFilterOrder, lfoCutoffDepths, modulationInterval);
		filterProcessor.prepare(spec);
	}
	// DELAY
	delayLineProcessor.setParameters(parameterSettings.bandParameters[bandID].bandDelay, parameterSettings.bandParameters[bandID].bandFeedforward, parameterSettings.bandParameters[bandID].bandFeedback, parameterSettings.bandParameters[bandID].bandBlend, lfoDelayDepths, modulationInterval);
	delayLineProcessor.prepare(spec);

	// PANORAMA
	panoramaProcessor.setParameters(parameterSettings.bandParameters[bandID].bandPanorama, lfoPanDepths, modulationInterval);
	panoramaProcessor.prepare(spec);
	// GAIN
	gainProcessor.setGain(parameterSettings.bandParameters[bandID].bandGain);
	gainProcessor.prepare(spec);

}

juce::dsp::AudioBlock<float> BandProcessor::processBlock(juce::dsp::ProcessContextNonReplacing<float>& processingContext, ProcessorParameters& parameterSettings, std::vector<juce::dsp::AudioBlock<float>>& lfoSignals)
{
	// AudioBlock pointing to the buffer where the output signal should be stored.
	juce::dsp::AudioBlock<float> processedBlock(processingContext.getOutputBlock());


	for (int ch = 0; ch < processingContext.getInputBlock().getNumChannels(); ++ch)
	{
		const float* inputData = processingContext.getInputBlock().getChannelPointer(ch);
		float* outputData = processedBlock.getChannelPointer(ch);
		// Copying the input signal to the output buffer which is then processed using each of the processors.
		for (int sample = 0; sample < processingContext.getInputBlock().getNumSamples(); ++sample)
		{
			outputData[sample] = inputData[sample];
		}
	}

	// The following vectors store pointers to the LFO depths for all parameters that can be modulated.
	std::vector<float*>lfoGainDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthGain,
	&parameterSettings.bandParameters[bandID].lfo2DepthGain, &parameterSettings.bandParameters[bandID].lfo3DepthGain };

	std::vector<float*>lfoDelayDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthDelay,
	&parameterSettings.bandParameters[bandID].lfo2DepthDelay, &parameterSettings.bandParameters[bandID].lfo3DepthDelay };

	std::vector<float*>lfoPanDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthPan,
	&parameterSettings.bandParameters[bandID].lfo2DepthPan,&parameterSettings.bandParameters[bandID].lfo3DepthPan };

	std::vector<float*>lfoCutoffDepths{ &parameterSettings.bandParameters[bandID].lfo1DepthCutoff,
	&parameterSettings.bandParameters[bandID].lfo2DepthCutoff, &parameterSettings.bandParameters[bandID].lfo3DepthCutoff };

	// Obtaining the correct modulation interval set in the GUI.
	int modulationInterval = getModulationInterval(parameterSettings.modulationUpdateInterval);

	// FILTER
	// If the filter type is set to none, filter processor methods will not be called at all.
	if (parameterSettings.bandParameters[bandID].bandFilterType != 8)
	{
		filterProcessor.setParameters(parameterSettings.bandParameters[bandID].bandFilterType, parameterSettings.bandParameters[bandID].bandFilterCutoff, parameterSettings.bandParameters[bandID].bandFilterQ, parameterSettings.bandParameters[bandID].bandFilterGain, parameterSettings.bandParameters[bandID].bandFilterOrder, lfoCutoffDepths, modulationInterval);
		filterProcessor.processBlock(processedBlock, lfoSignals);
	}
	// DELAY
	delayLineProcessor.setParameters(parameterSettings.bandParameters[bandID].bandDelay, parameterSettings.bandParameters[bandID].bandFeedforward, parameterSettings.bandParameters[bandID].bandFeedback, parameterSettings.bandParameters[bandID].bandBlend, lfoDelayDepths, modulationInterval);
	delayLineProcessor.processBlock(processedBlock, lfoSignals);

	// PANORAMA
	panoramaProcessor.setParameters(parameterSettings.bandParameters[bandID].bandPanorama, lfoPanDepths, modulationInterval); // TODO find out how to do this automatically
	panoramaProcessor.processBlock(processedBlock, lfoSignals); // TODO fix -6dB volume dip

	// GAIN
	gainProcessor.setGain(parameterSettings.bandParameters[bandID].bandGain);
	gainProcessor.setModulation(lfoGainDepths, modulationInterval);
	gainProcessor.processBlock(processedBlock, lfoSignals);

	return processedBlock;
}

int BandProcessor::getBandID()
{
	return bandID;
}

int BandProcessor::getModulationInterval(int selectedModulationInterval)
{
	int modulationInterval = 1;
	// Depending on item which is selected in the combo box,
	// correct modulation interval is assigned.
	switch (selectedModulationInterval)
	{
		case 1: modulationInterval = 1; break;
		case 2: modulationInterval = 8; break;
		case 3: modulationInterval = 16; break;
		case 4: modulationInterval = 32; break;
		case 5: modulationInterval = 64; break;
		case 6: modulationInterval = 128; break;
		case 7: modulationInterval = 256; break;
	}
	return modulationInterval;
}