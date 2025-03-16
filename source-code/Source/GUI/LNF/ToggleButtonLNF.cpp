/*
  ==============================================================================

    ToggleButtonLNF.cpp
    Created: 19 May 2024 2:00:10am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "ToggleButtonLNF.h"

ToggleButtonLNF::ToggleButtonLNF()
{
    // This method changes the visual style according to the object of the CustomLookAndFeel class.
    setLookAndFeel(&customLookAndFeel);
}

ToggleButtonLNF::~ToggleButtonLNF()
{
    setLookAndFeel(nullptr);
}

void ToggleButtonLNF::paint(juce::Graphics& g)
{
	ToggleButton::paint(g);
}
