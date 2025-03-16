/*
  ==============================================================================

    VerticalSliderLNF.h
    Created: 14 May 2024 9:38:55am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include "CustomLNF.h"

class VerticalSliderLNF : public juce::Slider
{
public:
    VerticalSliderLNF();
    ~VerticalSliderLNF();

    bool keyPressed(const juce::KeyPress& k) override;
private:
    // This object is used to change the visual style of this slider.
    CustomLNF customLookAndFeel;
};