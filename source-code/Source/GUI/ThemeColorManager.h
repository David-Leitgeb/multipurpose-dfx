/*
  ==============================================================================

    ThemeColorManager.h
    Created: 23 Jan 2024 4:58:45pm
    Author:  Bc. David Leitgeb

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


namespace ThemeColorManager
{
    // Main colour palette
    const juce::Colour mdmTrioDark = juce::Colour::fromRGBA(4, 1, 0, 255);
    const juce::Colour mdmTrioBright = juce::Colour::fromRGBA(220, 220, 220, 255);
    const juce::Colour mdmTrioColor = juce::Colour::fromRGBA(179, 33, 64, 255);
    const juce::Colour mdmTextWhite = juce::Colours::whitesmoke;

    
    const juce::Colour mdmViewCutoff = mdmTrioColor;//juce::Colours::darkred;a
    const juce::Colour mdmViewQ = juce::Colours::darkgreen;
    const juce::Colour mdmViewDelay = juce::Colours::darkslateblue;
    const juce::Colour mdmViewFeedback = juce::Colours::darksalmon;
    const juce::Colour mdmViewPanorama = juce::Colours::darkviolet;
    const juce::Colour mdmViewGain = juce::Colours::darkorange;
}