/*
  ==============================================================================

    VerticalSliderLNF.cpp
    Created: 14 May 2024 9:38:55am
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#include "VerticalSliderLNF.h"

VerticalSliderLNF::VerticalSliderLNF()
{
    // Changing the default settings of the slider.
	setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    setColour(juce::Slider::textBoxTextColourId, ThemeColorManager::mdmTextWhite);
    setColour(juce::Slider::textBoxOutlineColourId, ThemeColorManager::mdmTrioDark);


    // This method changes the visual style according to the object of the CustomLookAndFeel class.
    setLookAndFeel(&customLookAndFeel);
}

VerticalSliderLNF::~VerticalSliderLNF()
{
    setLookAndFeel(nullptr);
}

// This method is used to type in the slider values using the keyboard. Only selected keys are allowed.
bool VerticalSliderLNF::keyPressed(const juce::KeyPress& k)
{
    char numChars[] = "-0123456789";
    for (auto numChar : numChars)
    {
        if (k.isKeyCode(numChar))
        {
            CustomLNF::CustomLabel::initialPressedKey = juce::String::charToString(numChar);
            showTextBox();
            return true;
        }
    }
    return false;
}