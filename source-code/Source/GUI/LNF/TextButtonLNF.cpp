/*
  ==============================================================================

    TextButtonLNF.cpp
    Created: 13 May 2024 9:15:35pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "TextButtonLNF.h"

TextButtonLNF::TextButtonLNF()
{
    
}

TextButtonLNF::TextButtonLNF(const juce::String& buttonName)
{
    setButtonText(buttonName);
    setLookAndFeel(&customLookAndFeel);
}

TextButtonLNF::~TextButtonLNF()
{
    setLookAndFeel(nullptr);
}

void TextButtonLNF::paint(juce::Graphics& g)
{
	juce::TextButton::paint(g);
}
