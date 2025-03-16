/*
  ==============================================================================

    ToggleButtonLNF.h
    Created: 19 May 2024 2:00:10am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "CustomLNF.h"

class ToggleButtonLNF : public juce::ToggleButton
{
public:
    ToggleButtonLNF();
    ~ToggleButtonLNF();

    void paint(juce::Graphics& g) override;

private:
    // This object is used to change the visual style of this combo box.
    CustomLNF customLookAndFeel;
};