/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DSP/BandProcessor.h"
#include "DSP/GainProcessor.h"
#include "DSP/LFOProcessor.h"
#include "ProcessorParameters.h"
#include "Service/PresetManager.h"

//==============================================================================
/**
*/
class MultiBandModulatorAudioProcessor : public juce::AudioProcessor
{
public:
	//==============================================================================
	MultiBandModulatorAudioProcessor();
	~MultiBandModulatorAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	// Preset manager
	Service::PresetManager& getPresetManager() { return *presetManager; }

	// This method defines all the parameters used in the plugin.
	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	// APVTS is used to store all the parameters.
	juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

private:
	// Preset manager
	std::unique_ptr<Service::PresetManager> presetManager;

	// These instances manage all the signal processing
	// for each band.
	BandProcessor band1Processor,
		band2Processor,
		band3Processor,
		band4Processor,
		band5Processor,
		band6Processor,
		band7Processor,
		band8Processor,
		band9Processor,
		band10Processor,
		band11Processor,
		band12Processor,
		band13Processor,
		band14Processor,
		band15Processor,
		band16Processor,
		band17Processor,
		band18Processor,
		band19Processor,
		band20Processor,
		band21Processor,
		band22Processor,
		band23Processor,
		band24Processor,
		band25Processor,
		band26Processor,
		band27Processor,
		band28Processor,
		band29Processor,
		band30Processor;

	// These instances generate each of the LFO signals.
	LFOProcessor lfo1Processor, lfo2Processor, lfo3Processor;
	// This variable is used to synchronise LFOs with DAW playback.
	bool hostIsPlaying = false;

	// This vector stores AudioBlock which points to
	// each LFO signal in the memory.
	std::vector<juce::dsp::AudioBlock<float>> lfoSignals;

	// This method returns a vector containing pointers to LFOProcessor
	// instances for easier manipulation in the processing code.
	std::vector<LFOProcessor*> getLFOProcessors();

	//This method returns a vector containing pointers to BandProcessor
	//instances for easier manipulation in the processing code.
	std::vector<BandProcessor*> getBandProcessors();

	// GainProcessor instances used to set the input/output gain.
	GainProcessor inputGainProcessor;
	GainProcessor outputGainProcessor;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiBandModulatorAudioProcessor)
};
