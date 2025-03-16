/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiBandModulatorAudioProcessor::MultiBandModulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	:	band1Processor(1), // Initialization of each band with correct band index.
		band2Processor(2),
		band3Processor(3),
		band4Processor(4),
		band5Processor(5),
		band6Processor(6),
		band7Processor(7),
		band8Processor(8),
		band9Processor(9),
		band10Processor(10),
		band11Processor(11),
		band12Processor(12),
		band13Processor(13),
		band14Processor(14),
		band15Processor(15),
		band16Processor(16),
		band17Processor(17),
		band18Processor(18),
		band19Processor(19),
		band20Processor(20),
		band21Processor(21),
		band22Processor(22),
		band23Processor(23),
		band24Processor(24),
		band25Processor(25),
		band26Processor(26),
		band27Processor(27),
		band28Processor(28),
		band29Processor(29),
		band30Processor(30),
		lfoSignals(3),
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	// Preset manager setup.
	apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
	apvts.state.setProperty("version", ProjectInfo::versionNumber, nullptr);
	presetManager = std::make_unique<Service::PresetManager>(apvts);
}

MultiBandModulatorAudioProcessor::~MultiBandModulatorAudioProcessor()
{
}

//==============================================================================
const juce::String MultiBandModulatorAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MultiBandModulatorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MultiBandModulatorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MultiBandModulatorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MultiBandModulatorAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MultiBandModulatorAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MultiBandModulatorAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MultiBandModulatorAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MultiBandModulatorAudioProcessor::getProgramName(int index)
{
	return {};
}

void MultiBandModulatorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MultiBandModulatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Loading the APVTS parameter values to variables inside the ProcessorParameters struct.
	auto parameterSettings = getParameterSettings(apvts);

	// Spec contains the basic audio setting information and is passed
	// to prepare methods of the individual processors.
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = getTotalNumOutputChannels();

	// LFO setup.
	int index = 0;
	for (auto* processor : getLFOProcessors())
	{
		processor->prepare(spec, samplesPerBlock);
		processor->setParameters(parameterSettings.lfoParameters[index].lfoShape, parameterSettings.lfoParameters[index].lfoRateSync, parameterSettings.lfoParameters[index].lfoRateHz, parameterSettings.lfoParameters[index].lfoDepth, parameterSettings.lfoParameters[index].lfoRateMode, 140.0f, false, 0.0f, false);
		index++;
	}

	// Input gain setup.
	inputGainProcessor.prepare(spec);
	inputGainProcessor.setGain(parameterSettings.globalInputGain);


	// Band processing setup.
	for (auto* processor : getBandProcessors())
	{
		processor->prepare(spec, parameterSettings);
	}

	// Output gain setup.
	outputGainProcessor.prepare(spec);
	outputGainProcessor.setGain(parameterSettings.globalOutputGain);
}

void MultiBandModulatorAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultiBandModulatorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MultiBandModulatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// Loading the APVTS parameter values to variables inside the ProcessorParameters struct.
	auto parameterSettings = getParameterSettings(apvts);

	// LFO processing:
	float bpm = 140.0f; // default BPM when host does not provide it
	// Retrieving the BPM value if the DAW provides it.
	if (getPlayHead() != nullptr && getPlayHead()->getPosition()->getBpm().hasValue())
	{
		bpm = *getPlayHead()->getPosition()->getBpm();
	}

	// Calculating the LFO phase depending on the DAW playhead position (used for synchronization).
	float hostLfoPhase = 0.0f;
	if (getPlayHead() != nullptr && getPlayHead()->getPosition()->getPpqPosition().hasValue() && getPlayHead()->getPosition()->getTimeSignature().hasValue())
	{
		float ppqPosition = *getPlayHead()->getPosition()->getPpqPosition();
		auto beatsPerBar = getPlayHead()->getPosition()->getTimeSignature()->numerator;
		hostLfoPhase = fmodf(ppqPosition / static_cast<float>(beatsPerBar), 1.0);
	}

	// Deciding if the internal LFO phase should be replaced with the one calculated above.
	bool hostWasPlaying = hostIsPlaying;
	hostIsPlaying = getPlayHead()->getPosition()->getIsPlaying();
	bool retriggerLFO = false;
	if (hostIsPlaying && !hostWasPlaying)
	{
		retriggerLFO = true;
	}

	// Generating the LFO signals:
	int index = 0;
	for (auto* processor : getLFOProcessors())
	{
		processor->setParameters(parameterSettings.lfoParameters[index].lfoShape, parameterSettings.lfoParameters[index].lfoRateSync, parameterSettings.lfoParameters[index].lfoRateHz, parameterSettings.lfoParameters[index].lfoDepth, parameterSettings.lfoParameters[index].lfoRateMode, bpm, hostIsPlaying, hostLfoPhase, retriggerLFO);
		lfoSignals[index] = processor->generateSignal();
		index++;
	}

	// This is where the actual signal processing takes place:

	// In order to be able to change the dry/wet mix of the effect, the input signal
	// is stored inside a separate buffer.
	juce::AudioBuffer<float> dryBuffer(buffer.getNumChannels(), buffer.getNumSamples());
	dryBuffer.clear();
	dryBuffer.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
	dryBuffer.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());

	// Input gain:
	juce::dsp::AudioBlock<float> audioBlock{ buffer };
	inputGainProcessor.setGain(parameterSettings.globalInputGain);
	inputGainProcessor.processBlock(audioBlock, lfoSignals);

	// Band processing:
	// In order to minimize the amount of situations where the buffer data is copied between memory,
	// the band processing system utilizes additional AudioBuffers and AudioBlocks.

	// This AudioBuffer and AudioBlock store the signal that is individually processed with each of
	// the bands.
	juce::AudioBuffer<float> bandInputBuffer(buffer.getNumChannels(), buffer.getNumSamples());
	bandInputBuffer.clear();
	bandInputBuffer.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
	bandInputBuffer.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
	juce::dsp::AudioBlock<float> bandInputBlock(bandInputBuffer);

	// This AudioBuffer and AudioBlock store the output signal of each band (it is continuously being
	// replaced as individual bands finish their processing).
	juce::AudioBuffer<float> bandOutputBuffer(buffer.getNumChannels(), buffer.getNumSamples());
	bandOutputBuffer.clear();
	juce::dsp::AudioBlock<float> bandOutputBlock(bandOutputBuffer);

	// Clearing the buffer the audioBlock is pointing to.
	// This AudioBlock points to the original buffer delivered
	// by the DAW to the processBlock method which also serves
	// as the output buffer.
	audioBlock.clear();

	// Setting up a ProcessContext that points to two buffers:
	// one contains the input signal that is processed by each of the band processors (same signal for all of them),
	// the second one contains the output signal of each band (these buffers are being added together)
	juce::dsp::ProcessContextNonReplacing<float> bandProcessingContext(bandInputBlock, bandOutputBlock);

	// Solo function implemetation:
	bool globalSoloState = false;
	for (auto* processor : getBandProcessors())
	{
		// This condition checks if any of the bands have solo activated.
		if (parameterSettings.bandParameters[processor->getBandID()].bandSolo)
		{
			globalSoloState = true;
			break;
		}
	}

	// Calling the processBlock method for each of the bands depending on their settings.
	for (auto* processor : getBandProcessors())
	{
		// The processing is done only for processors that are not bypassed or have solo activated.
		if (!parameterSettings.bandParameters[processor->getBandID()].bandBypass && (!globalSoloState || parameterSettings.bandParameters[processor->getBandID()].bandSolo))
		{
			// Output signal of each band is added to the buffer which was cleared earlier in the code.
			audioBlock.add(processor->processBlock(bandProcessingContext, parameterSettings, lfoSignals));
		}
	}

	// Output Gain:
	outputGainProcessor.setGain(parameterSettings.globalOutputGain);
	outputGainProcessor.processBlock(audioBlock, lfoSignals);

	// Global Feedback:
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		auto* channelDataDry = dryBuffer.getWritePointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			// Adding the input buffer to the processed signal.
			channelData[sample] += (parameterSettings.globalFeedforward / 100.0f) * channelDataDry[sample];
		}
	}

	// Mix
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		auto* channelDataDry = dryBuffer.getWritePointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{

			channelData[sample] = (parameterSettings.globalMix / 100.0f) * channelData[sample] + ((100.0f - parameterSettings.globalMix) / 100.0f) * channelDataDry[sample];
		}
	}
}

//==============================================================================
bool MultiBandModulatorAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiBandModulatorAudioProcessor::createEditor()
{
	return new MultiBandModulatorAudioProcessorEditor(*this);
	//return new juce::GenericAudioProcessorEditor(*this); // GENERIC EDITOR
}

//==============================================================================
void MultiBandModulatorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	// Preset manager settings
	const auto state = apvts.copyState();
	const auto xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void MultiBandModulatorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// Preset manager settings
	const auto xmlState = getXmlFromBinary(data, sizeInBytes);
	if (xmlState == nullptr)
		return;
	const auto newTree = juce::ValueTree::fromXml(*xmlState);
	apvts.replaceState(newTree);
}

// This method defines all the parameters used in the plugin.
juce::AudioProcessorValueTreeState::ParameterLayout
MultiBandModulatorAudioProcessor::createParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterFloat>("GlobalInputGain",
		"Input Gain",
		juce::NormalisableRange<float>(-60.0f, 10.0f, 0.01f),
		0.0f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("GlobalOutputGain",
		"Output Gain",
		juce::NormalisableRange<float>(-60.0f, 10.0f, 0.01f),
		0.0f));

	layout.add(std::make_unique<juce::AudioParameterInt>("GlobalMix",
		"Mix",
		0,
		100,
		100));

	layout.add(std::make_unique<juce::AudioParameterInt>("GlobalFeedforward",
		"Feedforward",
		0,
		100,
		0));

	layout.add(std::make_unique<juce::AudioParameterInt>("ModulationUpdateInterval",
		"Modulation update interval",
		0,
		7,
		5));

	// Parameters for each band are created inside this for loop.
	for (int i = 0; i < 30 /* BAND COUNT IS CHANGED HERE */; ++i)
	{
		layout.add(std::make_unique<juce::AudioParameterInt>("Band" + static_cast<juce::String>(i + 1) + "FilterType",
			"Type",
			1,
			8,
			7));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "FilterCutoff",
			"Cutoff",
			juce::NormalisableRange<float>(10.0f, 22000.0f, 1.0f, 0.25f),
			22000.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "FilterQ",
			"Q",
			juce::NormalisableRange<float>(0.10f, 18.0f, 0.01f),
			1.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "FilterGain",
			"Gain",
			juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterInt>("Band" + static_cast<juce::String>(i + 1) + "FilterOrder",
			"Order",
			1,
			2,
			2));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "Delay",
			"Delay",
			juce::NormalisableRange<float>(1.0f, 1000.0f, 0.01f, 0.25f),
			1.0f));

		layout.add(std::make_unique<juce::AudioParameterInt>("Band" + static_cast<juce::String>(i + 1) + "Feedback",
			"Feedback",
			0,
			95,
			0));

		layout.add(std::make_unique<juce::AudioParameterInt>("Band" + static_cast<juce::String>(i + 1) + "Feedforward",
			"Feedforward",
			0,
			100,
			100));

		layout.add(std::make_unique<juce::AudioParameterInt>("Band" + static_cast<juce::String>(i + 1) + "Blend",
			"Blend",
			0,
			100,
			0));

		layout.add(std::make_unique<juce::AudioParameterInt>("Band" + static_cast<juce::String>(i + 1) + "Panorama",
			"Panorama",
			-50,
			50,
			0));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "Gain",
			"Gain",
			juce::NormalisableRange<float>(-60.0f, 10.0f, 0.01f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterBool>("Band" + static_cast<juce::String>(i + 1) + "Bypass", "Bypass", true));

		layout.add(std::make_unique<juce::AudioParameterBool>("Band" + static_cast<juce::String>(i + 1) + "Solo", "Solo", false));

		// LFO modulation parameters
		// Cutoff
		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthCutoff",
			"LFO1 Cutoff Depth",
			juce::NormalisableRange<float>(0.0f, 22000.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthCutoff",
			"LFO2 Cutoff Depth",
			juce::NormalisableRange<float>(0.0f, 22000.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthCutoff",
			"LFO3 Cutoff Depth",
			juce::NormalisableRange<float>(0.0f, 22000.0f, 0.01f, 0.25f),
			0.0f));
		// Gain
		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthGain",
			"LFO1 Gain Depth",
			juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthGain",
			"LFO2 Gain Depth",
			juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthGain",
			"LFO3 Gain Depth",
			juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.25f),
			0.0f));
		// Delay
		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthDelay",
			"LFO1 Delay Depth",
			juce::NormalisableRange<float>(0.0f, 1000.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthDelay",
			"LFO2 Delay Depth",
			juce::NormalisableRange<float>(0.0f, 1000.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthDelay",
			"LFO3 Delay Depth",
			juce::NormalisableRange<float>(0.0f, 1000.0f, 0.01f, 0.25f),
			0.0f));
		// Pan
		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO1DepthPan",
			"LFO1 Pan Depth",
			juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO2DepthPan",
			"LFO2 Pan Depth",
			juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.25f),
			0.0f));

		layout.add(std::make_unique<juce::AudioParameterFloat>("Band" + static_cast<juce::String>(i + 1) + "LFO3DepthPan",
			"LFO3 Pan Depth",
			juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.25f),
			0.0f));

	}

	// LFO parameters
	for (int i = 0; i < 3 /*LFO COUNT IS CHANGED HERE*/; ++i)
	{
		layout.add(std::make_unique<juce::AudioParameterInt>("LFO" + static_cast<juce::String>(i + 1) + "Shape",
			"Shape",
			1,
			5,
			1));

		layout.add(std::make_unique<juce::AudioParameterInt>("LFO" + static_cast<juce::String>(i + 1) + "RateSync",
			"Rate",
			1,
			6,
			3));

		layout.add(std::make_unique<juce::AudioParameterFloat>("LFO" + static_cast<juce::String>(i + 1) + "RateHz",
			"Rate",
			juce::NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.25f),
			1.0f));

		layout.add(std::make_unique<juce::AudioParameterBool>("LFO" + static_cast<juce::String>(i + 1) + "RateMode",
			"Sync",
			false));

		layout.add(std::make_unique<juce::AudioParameterInt>("LFO" + static_cast<juce::String>(i + 1) + "Depth",
			"Depth",
			0,
			100,
			100));
	}

	return layout;
}

std::vector<LFOProcessor*> MultiBandModulatorAudioProcessor::getLFOProcessors()
{
	return
	{
		&lfo1Processor,
		&lfo2Processor,
		&lfo3Processor,
	};
}

std::vector<BandProcessor*> MultiBandModulatorAudioProcessor::getBandProcessors()
{
	return
	{
		&band1Processor,
		&band2Processor,
		&band3Processor,
		&band4Processor,
		&band5Processor,
		&band6Processor,
		&band7Processor,
		&band8Processor,
		&band9Processor,
		&band10Processor,
		&band11Processor,
		&band12Processor,
		&band13Processor,
		&band14Processor,
		&band15Processor,
		&band16Processor,
		&band17Processor,
		&band18Processor,
		&band19Processor,
		&band20Processor,
		&band21Processor,
		&band22Processor,
		&band23Processor,
		&band24Processor,
		&band25Processor,
		&band26Processor,
		&band27Processor,
		&band28Processor,
		&band29Processor,
		&band30Processor
	};

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MultiBandModulatorAudioProcessor();
}
