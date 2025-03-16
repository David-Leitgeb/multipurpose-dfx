/*
  ==============================================================================

    SliderLNF.h
    Created: 7 Feb 2022 6:49:38pm
    Author:  David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLNF.h"

class SliderLNF : public juce::Slider
{
public:
    SliderLNF();
    ~SliderLNF();

    void paint(juce::Graphics& g) override;
    bool keyPressed(const juce::KeyPress& k) override;
private:
    // This object is used to change the visual style of this slider.
    CustomLNF customLookAndFeel;
};