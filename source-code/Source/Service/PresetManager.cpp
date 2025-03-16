/*
  ==============================================================================

    PresetManager.cpp
    Created: 11 May 2024 10:55:52am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "PresetManager.h"

namespace Service
{
    // Default location for the presets is set to be created in the documents folder
    const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory)
        .getChildFile(ProjectInfo::companyName)
        .getChildFile(ProjectInfo::projectName) };

    // Used file extension
    const juce::String PresetManager::extension{ "preset" };

    const juce::String PresetManager::presetNameProperty{ "presetName" };

    PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts)
    {
        // Default preset directory is created if it does not already exist.
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            // In the debug mode an error will be triggered when the directory cannot be created.
            if (result.failed())
            {
                DBG("Could not create preset directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }

        // Connecting the currentPreset variable with the preset name inside the file.
        // Changing one of these will also update the other one.
        valueTreeState.state.addListener(this);
        currentPreset.referTo(valueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));
    }

    void PresetManager::savePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
            return;

        currentPreset.setValue(presetName);

        const auto xml = valueTreeState.copyState().createXml();
        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!xml->writeTo(presetFile))
        {
            DBG("Could not create preset file: " + presetFile.getFullPathName());
            jassertfalse;
        }
    }

    void PresetManager::deletePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file" + presetFile.getFullPathName() + "does not exist");
            jassertfalse;
            // If the file does not exist, the rest of the method will not be executed.
            return;
        }

        // Preset file exists
        if (!presetFile.deleteFile())
        {
            DBG("Preset file" + presetFile.getFullPathName() + "could not be deleted");
            jassertfalse;
            return;
        }

        currentPreset.setValue("");
    }

    void PresetManager::loadPreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file" + presetFile.getFullPathName() + "does not exist");
            jassertfalse;
            // If the file does not exist, the rest of the method will not be executed.
            return;
        }

        // The preset XML file has to be converted to the ValueTree format.
        juce::XmlDocument xmlDocument{ presetFile };
        const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());

        valueTreeState.replaceState(valueTreeToLoad);
        currentPreset.setValue(presetName);
    }

    int PresetManager::loadNextPreset()
    {
        const auto allPresets = getAllPresets();
        if (allPresets.isEmpty())
            // Returning -1 will set the combo box to "not selected" state
            return -1;

        const auto currentIndex = allPresets.indexOf((currentPreset.toString()));
        // If the end of the preset list is reached, the first preset from the list will be selected
        const auto nextIndex = currentIndex + 1 > (allPresets.size() - 1) ? 0 : currentIndex + 1;

        loadPreset(allPresets.getReference(nextIndex));
        return nextIndex;
    }

    int PresetManager::loadPreviousPreset()
    {
        const auto allPresets = getAllPresets();
        if (allPresets.isEmpty())
            // Returning -1 will set the combo box to "not selected" state
            return -1;

        const auto currentIndex = allPresets.indexOf((currentPreset.toString()));
        // If the start of the preset list is reached, the last preset from the list will be selected
        const auto previousIndex = currentIndex - 1 < 0 ? allPresets.size() - 1 : currentIndex - 1;

        loadPreset(allPresets.getReference(previousIndex));
        return previousIndex;
    }

    juce::StringArray PresetManager::getAllPresets() const
    {
        juce::StringArray presets;
        const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);

        for (const auto& file : fileArray)
        {
            presets.add(file.getFileNameWithoutExtension());
        }
        return presets;
    }

    juce::String PresetManager::getCurrentPreset() const
    {
        return currentPreset.toString();
    }

    void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
    {
        currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
    }
}
