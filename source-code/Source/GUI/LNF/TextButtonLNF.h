/*
  ==============================================================================

    TextButtonLNF.h
    Created: 13 May 2024 9:15:35pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "../ThemeColorManager.h"
#include "CustomLNF.h"

class TextButtonLNF : public juce::TextButton
{
public:
    TextButtonLNF();
    TextButtonLNF(const juce::String& buttonName);
    ~TextButtonLNF();

    void paint(juce::Graphics& g) override;

private:
    // This object is used to change the visual style of this slider.
    CustomLNF customLookAndFeel;
};