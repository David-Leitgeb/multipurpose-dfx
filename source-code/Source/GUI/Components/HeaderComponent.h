/*
  ==============================================================================

    HeaderComponent.h
    Created: 30 Dec 2023 3:18:24pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Service/PresetManager.h"
#include "../LNF/TextButtonLNF.h"
#include "../LNF/ComboBoxLNF.h"

//==============================================================================
/*
*/
class HeaderComponent  : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
{
public:
    HeaderComponent(Service::PresetManager& pm);
    ~HeaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    

private:
    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    // This button changes the parameters and behaviour of given button.
    void buttonConfig(juce::Button& button);
    // This method mantains correct content of the preset combo box.
    void loadPresetList();

    // Preset manager settings:
    Service::PresetManager& presetManager;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Components
    TextButtonLNF buttonSavePreset{ "Save" },
        buttonDeletePreset{ "Delete" },
        buttonPreviousPreset{ "<" },
        buttonNextPreset{ ">" };

    ComboBoxLNF comboBoxSelectedPreset;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};