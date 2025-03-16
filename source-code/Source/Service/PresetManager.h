/*
  ==============================================================================

	PresetManager.h
	Created: 11 May 2024 10:55:52am
	Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
namespace Service
{
	class PresetManager : juce::ValueTree::Listener
	{
	public:
		// File related parameters
		static const juce::File defaultDirectory;
		static const juce::String extension;
		static const juce::String presetNameProperty;
		PresetManager(juce::AudioProcessorValueTreeState&);

		// Methods that manipulate the presets
		void savePreset(const juce::String& presetName);
		void deletePreset(const juce::String& presetName);
		void loadPreset(const juce::String& presetName);
		int loadNextPreset();
		int loadPreviousPreset();
		juce::StringArray getAllPresets() const;
		juce::String getCurrentPreset() const;

	private:
		void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override;
		juce::AudioProcessorValueTreeState& valueTreeState;
		juce::Value currentPreset;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
	};
}