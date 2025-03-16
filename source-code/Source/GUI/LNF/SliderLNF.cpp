/*
  ==============================================================================

    SliderLNF.cpp
    Created: 7 Feb 2022 6:49:45pm
    Author:  David Leitgeb

  ==============================================================================
*/

#include "SliderLNF.h"

SliderLNF::SliderLNF()
{
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                        juce::MathConstants<float>::pi * 2.75f,
                        true);
    setColour(juce::Slider::textBoxTextColourId, ThemeColorManager::mdmTextWhite);
    setColour(juce::Slider::textBoxOutlineColourId, ThemeColorManager::mdmTrioDark);
    setColour(juce::Slider::rotarySliderFillColourId, ThemeColorManager::mdmTrioColor);

    setVelocityBasedMode(true);
    setVelocityModeParameters(1.0, 1, 0.1, false);
    onValueChange = [&]()
    {
        if (getValue() < 10)
            setNumDecimalPlacesToDisplay(2);
        else if (10 <= getValue() && getValue() < 100)
            setNumDecimalPlacesToDisplay(1);
        else
            setNumDecimalPlacesToDisplay(0);
    };
    // This method changes the visual style according to the object of the CustomLookAndFeel class.
    setLookAndFeel(&customLookAndFeel);
}

void SliderLNF::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);

    // Highlighting selected slider with gradient background.
    if (hasKeyboardFocus(true))
    {
        auto localBounds = getLocalBounds().withTrimmedTop(getLocalBounds().getWidth() * 0.2f);
        auto borderSize = localBounds.getWidth() * 0.2f;
        auto bounds = localBounds.withTrimmedBottom(borderSize).withTrimmedLeft(borderSize).withTrimmedRight(borderSize).toFloat();

        g.setColour(findColour(juce::Slider::textBoxOutlineColourId));

        // Gradient
        juce::ColourGradient focusGradient(findColour(juce::Slider::rotarySliderFillColourId).darker(0.3f).withAlpha(0.5f), bounds.getCentre().getX(), bounds.getCentre().getY(),
                                           findColour(juce::Slider::rotarySliderFillColourId).brighter(0.3f).withAlpha(0.5f), bounds.getCentre().getX() + 15, bounds.getCentre().getY() + 15.0f, false);
        
        // Defining and painting two gradients which are combined to create a background for the highlighted slider.
        juce::ColourGradient backgroundGradient(findColour(juce::Slider::rotarySliderFillColourId).withAlpha(0.20f), localBounds.getCentreX(), localBounds.getBottom() + 20.0f,
                                                ThemeColorManager::mdmTrioDark.brighter(0.4f).withAlpha(0.0f), localBounds.getCentreX(), localBounds.getY() + 10.0f, true);
        g.setGradientFill(backgroundGradient);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.0f);
        juce::ColourGradient gradientWhite(juce::Colours::white.withAlpha(0.05f), localBounds.getCentreX() - 10.0f, localBounds.getY() - 20.0f,
            ThemeColorManager::mdmTrioDark.withAlpha(0.0f), localBounds.getCentreX(), localBounds.getBottom() + 10.0f, true);
        g.setGradientFill(gradientWhite);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.0f);
    }
}



SliderLNF::~SliderLNF()
{
    setLookAndFeel(nullptr);
}

// This method is used to type in the slider values using the keyboard. Only selected keys are allowed.
bool SliderLNF::keyPressed(const juce::KeyPress& k)
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