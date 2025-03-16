/*
  ==============================================================================

	ProcessorParameters.h
	Created: 6 Apr 2024 11:48:08am
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

// Number of bands and LFOs in the plugin.
static int numBands = 30;
static int numLfos = 3;

// This struct contains all the variables that are used
// to hold each of the APVTS parameters.
struct ProcessorParameters
{
	// Global parameters of the whole plugin.
	float globalInputGain{ 0.0f }, globalOutputGain{ 0.0f };
	int globalMix{ 100 }, globalFeedforward{ 0 }, modulationUpdateInterval{ 5 };

	// This struct is used to store parameters for each of the bands.
	struct BandParameters
	{
		// Band parameters
		float bandFilterCutoff{ 22000.0f }, bandFilterQ{ 1.0f }, bandFilterGain{ 0.0f }, bandDelay{ 1.0f }, bandGain{ 0.0f };
		int bandFilterType{ 7 }, bandFilterOrder{ 2 }, bandFeedback{ 0 }, bandFeedforward{ 100 }, bandBlend{ 0 }, bandPanorama{ 0 };
		bool bandBypass{ true }, bandSolo{ false };

		// LFO depth parameters
		float lfo1DepthGain{ 0.0f }, lfo2DepthGain{ 0.0f }, lfo3DepthGain{ 0.0f },
			lfo1DepthDelay{ 0.0f }, lfo2DepthDelay{ 0.0f }, lfo3DepthDelay{ 0.0f },
			lfo1DepthPan{ 0.0f }, lfo2DepthPan{ 0.0f }, lfo3DepthPan{ 0.0f },
			lfo1DepthCutoff{ 0.0f }, lfo2DepthCutoff{ 0.0f }, lfo3DepthCutoff{ 0.0f };
	};
	// This struct is used to store parameters for each of the LFOs.
	struct LFOParameters
	{
		int lfoShape{ 1 }, lfoRateSync{ 3 }, lfoDepth{ 100 };
		float lfoRateHz{ 1.0f };
		bool lfoRateMode{ false };
	};

	// These vectors store parameters for all the bands and LFOs.
	std::vector<BandParameters> bandParameters;
	std::vector<LFOParameters> lfoParameters;

	// 
	ProcessorParameters() : bandParameters(numBands), lfoParameters(numLfos)
	{
		// Initialization of the band parameters.
		for (int i = 0; i < numBands; ++i)
		{
			bandParameters[i] = BandParameters{};
		}
		// Initialization of the LFO parameters.
		for (int i = 0; i < numLfos; ++i)
		{
			lfoParameters[i] = LFOParameters{};
		}
	}
};

// This method returns all the parameters stored in the struct ProcessorParameters.
ProcessorParameters getParameterSettings(juce::AudioProcessorValueTreeState& apvts);