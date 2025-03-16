/*
  ==============================================================================

    ComboBoxLNF.cpp
    Created: 18 May 2024 11:05:35pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "ComboBoxLNF.h"

ComboBoxLNF::ComboBoxLNF()
{
    // This method changes the visual style according to the object of the CustomLookAndFeel class.
    setLookAndFeel(&customLookAndFeel);
}

ComboBoxLNF::~ComboBoxLNF()
{
    setLookAndFeel(nullptr);
}

void ComboBoxLNF::paint(juce::Graphics& g)
{
    juce::ComboBox::paint(g);
}
