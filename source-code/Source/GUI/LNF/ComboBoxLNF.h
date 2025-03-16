/*
  ==============================================================================

    ComboBoxLNF.h
    Created: 18 May 2024 11:05:35pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "CustomLNF.h"

class ComboBoxLNF : public juce::ComboBox
{
public:
    ComboBoxLNF();
    ~ComboBoxLNF();

    void paint(juce::Graphics& g) override;

private:
    // This object is used to change the visual style of this combo box.
    CustomLNF customLookAndFeel;
};