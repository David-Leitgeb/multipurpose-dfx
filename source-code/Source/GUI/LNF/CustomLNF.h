/*
  ==============================================================================

    CustomLNF.h
    Created: 9 Feb 2022 3:00:43pm
    Author:  David Leitgeb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ThemeColorManager.h"

class CustomLNF : public juce::LookAndFeel_V4
{
public:
    CustomLNF();
    ~CustomLNF();

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;

    void drawLabel(juce::Graphics&, juce::Label&) override;

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider&) override;

    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawComboBox(juce::Graphics&, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox&) override;

    void drawPopupMenuBackground(juce::Graphics&, int width, int height) override;

    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    struct CustomLabel : public juce::Label
    {
        static juce::String initialPressedKey;

        // Customized text editor used when the value of the slider is entered using the keyboard.
        juce::TextEditor* createEditorComponent() override
        {
            auto* editor = juce::Label::createEditorComponent();
            editor->setJustification(juce::Justification::centred);
            editor->setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
            editor->setInputRestrictions(5, "-0123456789.");
            editor->setIndents(4, 0);
            return editor;
        }
        // What happens when the text editor is opened.
        void editorShown(juce::TextEditor* editor) override
        {
            // Hiding the cursor while typing.
            getParentComponent()->setMouseCursor(juce::MouseCursor::NoCursor);
            editor->clear();
            // This makes sure the first pressed key is captured, without this line the first key
            // would only open the text editor.
            editor->setText(initialPressedKey);
        }
        // Setting the cursor to be visible after the editing.
        void editorAboutToBeHidden(juce::TextEditor* ed) override
        {
            getParentComponent()->setMouseCursor(juce::MouseCursor::NormalCursor);
        }
    };
    
    CustomLabel* createSliderTextBox(juce::Slider& slider) override;

    juce::CaretComponent* createCaretComponent(juce::Component* keyFocusOwner) override;

};