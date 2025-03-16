/*
  ==============================================================================

    ComponentLabel.h
    Created: 18 Mar 2024 8:45:13pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ThemeColorManager.h"

class ComponentLabel : public juce::Label
{
public:
	ComponentLabel(const juce::String& componentName)
	{
		// Adjusting the settings of the label.
		setColour(juce::Label::textColourId, ThemeColorManager::mdmTextWhite);
		setJustificationType(fontJustification);
		setFont(juce::Font("Segoe UI", 16.0f, juce::Font::plain));
		setText(componentName, juce::NotificationType::dontSendNotification);
	}
	~ComponentLabel() {}

	void resized() override
	{
		const int fontSize = std::round(getLocalBounds().toFloat().getHeight() * 0.01 * 60);
		setFont(juce::Font("Segoe UI", fontSize, juce::Font::plain));
		setJustificationType(fontJustification);
	}

	void setJustification(juce::Justification newJustification)
	{
		fontJustification = newJustification;
	}

private:
	juce::Justification fontJustification = juce::Justification::centred;
};