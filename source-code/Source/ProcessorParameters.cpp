/*
  ==============================================================================

    ProcessorParameters.cpp
    Created: 6 Apr 2024 11:48:08am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "ProcessorParameters.h"

// This method returns all the parameters stored in the struct ProcessorParameters.
// Parameter values are obtained from the APVTS reference.
ProcessorParameters getParameterSettings(juce::AudioProcessorValueTreeState& apvts)
{
	ProcessorParameters settings;

	// Global parameters
	settings.globalInputGain = apvts.getRawParameterValue("GlobalInputGain")->load();
	settings.globalOutputGain = apvts.getRawParameterValue("GlobalOutputGain")->load();
	settings.globalMix = apvts.getRawParameterValue("GlobalMix")->load();
	settings.globalFeedforward = apvts.getRawParameterValue("GlobalFeedforward")->load();
	settings.modulationUpdateInterval = apvts.getRawParameterValue("ModulationUpdateInterval")->load();

	// Bands
	for (int i = 0; i < numBands; ++i)
	{
		// Band parameters
		settings.bandParameters[i].bandFilterType = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "FilterType")->load();
		settings.bandParameters[i].bandFilterCutoff = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "FilterCutoff")->load();
		settings.bandParameters[i].bandFilterQ = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "FilterQ")->load();
		settings.bandParameters[i].bandFilterGain = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "FilterGain")->load();
		settings.bandParameters[i].bandFilterOrder = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "FilterOrder")->load();
		settings.bandParameters[i].bandDelay = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Delay")->load();
		settings.bandParameters[i].bandFeedback = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Feedback")->load();
		settings.bandParameters[i].bandFeedforward = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Feedforward")->load();
		settings.bandParameters[i].bandBlend = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Blend")->load();
		settings.bandParameters[i].bandPanorama = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Panorama")->load();
		settings.bandParameters[i].bandGain = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Gain")->load();
		settings.bandParameters[i].bandBypass = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Bypass")->load();
		settings.bandParameters[i].bandSolo = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "Solo")->load();
		// LFO depth parameters
		settings.bandParameters[i].lfo1DepthGain = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthGain")->load();
		settings.bandParameters[i].lfo2DepthGain = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthGain")->load();
		settings.bandParameters[i].lfo3DepthGain = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthGain")->load();
		settings.bandParameters[i].lfo1DepthDelay = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthDelay")->load();
		settings.bandParameters[i].lfo2DepthDelay = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthDelay")->load();
		settings.bandParameters[i].lfo3DepthDelay = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthDelay")->load();
		settings.bandParameters[i].lfo1DepthPan = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthPan")->load();
		settings.bandParameters[i].lfo2DepthPan = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthPan")->load();
		settings.bandParameters[i].lfo3DepthPan = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthPan")->load();
		settings.bandParameters[i].lfo1DepthCutoff = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthCutoff")->load();
		settings.bandParameters[i].lfo2DepthCutoff = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthCutoff")->load();
		settings.bandParameters[i].lfo3DepthCutoff = apvts.getRawParameterValue("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthCutoff")->load();
	}

	// LFO parameters
	for (int i = 0; i < numLfos; ++i)
	{
		settings.lfoParameters[i].lfoShape = apvts.getRawParameterValue("LFO" + static_cast<juce::String>(i + 1) + "Shape")->load();
		settings.lfoParameters[i].lfoRateSync = apvts.getRawParameterValue("LFO" + static_cast<juce::String>(i + 1) + "RateSync")->load();
		settings.lfoParameters[i].lfoRateHz = apvts.getRawParameterValue("LFO" + static_cast<juce::String>(i + 1) + "RateHz")->load();
		settings.lfoParameters[i].lfoRateMode = apvts.getRawParameterValue("LFO" + static_cast<juce::String>(i + 1) + "RateMode")->load();
		settings.lfoParameters[i].lfoDepth = apvts.getRawParameterValue("LFO" + static_cast<juce::String>(i + 1) + "Depth")->load();
	}

	return settings;
}