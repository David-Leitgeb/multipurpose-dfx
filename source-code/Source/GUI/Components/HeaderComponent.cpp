/*
  ==============================================================================

    HeaderComponent.cpp
    Created: 30 Dec 2023 3:18:24pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HeaderComponent.h"

//==============================================================================
HeaderComponent::HeaderComponent(Service::PresetManager& pm) : presetManager(pm)
{
    // Adjusting the preset button parameters and behaviour.
	buttonConfig(buttonSavePreset);
    buttonConfig(buttonDeletePreset);
    buttonConfig(buttonNextPreset);
    buttonConfig(buttonPreviousPreset);

    // Setting up the combo box.
    comboBoxSelectedPreset.setColour(juce::ComboBox::backgroundColourId, ThemeColorManager::mdmTrioColor.darker(0.5f));
    comboBoxSelectedPreset.setTextWhenNothingSelected("No preset selected");
    comboBoxSelectedPreset.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(comboBoxSelectedPreset);
    comboBoxSelectedPreset.addListener(this);

    loadPresetList();
}

HeaderComponent::~HeaderComponent()
{
    // These listeners which are added in the
    // constructor call the buttonClicked method.
    buttonSavePreset.removeListener(this);
    buttonDeletePreset.removeListener(this);
    buttonNextPreset.removeListener(this);
    buttonPreviousPreset.removeListener(this);
    comboBoxSelectedPreset.removeListener(this);
}

void HeaderComponent::paint (juce::Graphics& g)
{
    // Background
    g.fillAll (ThemeColorManager::mdmTrioDark.brighter(0.15f).withSaturation(0.0f));

    juce::Rectangle<int> rectHeaderBounds = getLocalBounds();
    g.setFont(juce::Font("Trebuchet MS", rectHeaderBounds.getHeight() * 0.65f, juce::Font::bold));
    // Shadow
    g.setColour(ThemeColorManager::mdmTrioDark.withAlpha(0.6f));
    float shadowOffset = rectHeaderBounds.getHeight() * 0.07;
    g.drawText("MultiBandModulator", rectHeaderBounds.withTrimmedLeft(getWidth() * 0.01).translated(shadowOffset, shadowOffset), juce::Justification::centredLeft, true);
    // Logo
	g.setColour(ThemeColorManager::mdmTrioColor);
    g.drawText("MultiBandModulator", rectHeaderBounds.withTrimmedLeft(getWidth() * 0.01), juce::Justification::centredLeft, true);
}

void HeaderComponent::resized()
{
    // Defining the rectangles which are then used as bounds for components.
    juce::Rectangle<int> rectHeader = getLocalBounds(),
        rectDeletePreset = rectHeader.removeFromRight(rectHeader.proportionOfHeight(3.0f)),
        rectSavePreset = rectHeader.removeFromRight(rectHeader.proportionOfHeight(3.0f)),
        rectSelectedPreset = rectHeader.removeFromRight(rectHeader.proportionOfHeight(10.0f)),
        rectNextPreset = rectHeader.removeFromRight(rectHeader.getHeight()),
        rectPreviousPreset = rectHeader.removeFromRight(rectHeader.getHeight()),
        rectLogo = rectHeader;

    float reduceSize = rectLogo.getHeight() * 0.15f;

    // Setting up the bounds for each component.
    buttonDeletePreset.setBounds(rectDeletePreset.reduced(reduceSize));
    buttonSavePreset.setBounds(rectSavePreset.reduced(reduceSize));
    comboBoxSelectedPreset.setBounds(rectSelectedPreset.reduced(reduceSize));
    buttonNextPreset.setBounds(rectNextPreset.reduced(reduceSize));
    buttonPreviousPreset.setBounds(rectPreviousPreset.reduced(reduceSize));
}

void HeaderComponent::buttonClicked(juce::Button* button)
{
    if (button == &buttonSavePreset)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
        
            "Save preset",
            Service::PresetManager::defaultDirectory,
            "*." + Service::PresetManager::extension
        );

        fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
        {
                const auto resultFile = chooser.getResult();
                presetManager.savePreset(resultFile.getFileNameWithoutExtension());
                loadPresetList();
        });
	}

    if (button == &buttonPreviousPreset)
    {
        const auto index = presetManager.loadPreviousPreset();
        comboBoxSelectedPreset.setSelectedItemIndex(index, juce::dontSendNotification);
    }

    if (button == &buttonNextPreset)
    {
        const auto index = presetManager.loadNextPreset();
        comboBoxSelectedPreset.setSelectedItemIndex(index, juce::dontSendNotification);
    }

    if (button == &buttonDeletePreset)
    {
        // There is usually a prompt that asks the user if the preset should really be deleted.
        presetManager.deletePreset(presetManager.getCurrentPreset());
        loadPresetList();
    }
}

void HeaderComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &comboBoxSelectedPreset)
	{
        presetManager.loadPreset(comboBoxSelectedPreset.getItemText(comboBoxSelectedPreset.getSelectedItemIndex()));
	}
}


void HeaderComponent::buttonConfig(juce::Button& button)
{
    button.setColour(juce::TextButton::ColourIds::buttonColourId, ThemeColorManager::mdmTrioColor.darker(0.5f));
    button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(button);
    button.addListener(this);
}

void HeaderComponent::loadPresetList()
{
    comboBoxSelectedPreset.clear(juce::dontSendNotification);
    const auto allPresets = presetManager.getAllPresets();
    const auto currentPreset = presetManager.getCurrentPreset();
    comboBoxSelectedPreset.addItemList(allPresets, 1);
    comboBoxSelectedPreset.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
}
