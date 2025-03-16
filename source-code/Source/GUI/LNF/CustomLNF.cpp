/*
  ==============================================================================

	CustomLNF.cpp
	Created: 9 Feb 2022 3:10:52pm
	Author:  David Leitgeb

  ==============================================================================
*/

#include "CustomLNF.h"

CustomLNF::CustomLNF() {};
CustomLNF::~CustomLNF() {};

// Used for the text editor to make sure the first entered value is captured.
juce::String CustomLNF::CustomLabel::initialPressedKey = "";

// This struct defines the slider area and the text box area in context of the bounds assigned to the slider in the PluginEditor class.
juce::Slider::SliderLayout CustomLNF::getSliderLayout(juce::Slider& slider)
{
	int borderSize = slider.getLocalBounds().getWidth() * 0.2;
	auto localBounds = slider.getLocalBounds().withTrimmedTop(borderSize);
	juce::Slider::SliderLayout layout;

	if (slider.isRotary())
	{
	layout.textBoxBounds = localBounds.removeFromBottom(borderSize);
	layout.sliderBounds = localBounds.withTrimmedLeft(borderSize).withTrimmedRight(borderSize);
	}
	else if (slider.isBar() && slider.isVertical())
	{
		layout.textBoxBounds = localBounds.removeFromBottom(borderSize * .0f);
		layout.sliderBounds = localBounds.withTrimmedLeft(borderSize).withTrimmedRight(borderSize);
	}
	return layout;
}

// This method defines the way a rotary slider is drawn.
void CustomLNF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
	auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(2.0f);
	auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
	auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	auto lineW = radius * 0.085f;
	auto arcRadius = radius - lineW * 1.6f;

	juce::Path backgroundArc;
	backgroundArc.addCentredArc(bounds.getCentreX(),
		bounds.getCentreY(),
		arcRadius,
		arcRadius,
		0.0f,
		rotaryStartAngle,
		rotaryEndAngle,
		true);

	g.setColour(ThemeColorManager::mdmTrioDark.brighter(0.1f));
	g.strokePath(backgroundArc, juce::PathStrokeType(lineW + 2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	juce::Path valueArc;
	valueArc.addCentredArc(bounds.getCentreX(),
		bounds.getCentreY(),
		arcRadius,
		arcRadius,
		0.0f,
		rotaryStartAngle,
		toAngle,
		true);

	// Gradient used for the value arc of the slider.
	juce::ColourGradient gradient(slider.findColour(juce::Slider::rotarySliderFillColourId).darker(0.3f), backgroundArc.getBounds().getCentre().getX(), backgroundArc.getBounds().getCentre().getY(),
		slider.findColour(juce::Slider::rotarySliderFillColourId).brighter(0.3f), backgroundArc.getBounds().getCentre().getX() - 15, backgroundArc.getBounds().getCentre().getY() - 15, false);
	g.setGradientFill(gradient);
	g.strokePath(valueArc, juce::PathStrokeType(lineW + 2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	// Main shape of the slider.
	juce::Path stick;
	auto stickWidth = lineW * 1.25f;
	auto stickHeightClip = 8.0f;
	stick.addRoundedRectangle(-stickWidth / 2.0f, -stickWidth / 2.0f + stickHeightClip, stickWidth, radius - 3.4f * lineW - stickHeightClip, stickWidth / 2.0f);
	g.setColour(ThemeColorManager::mdmTextWhite.darker(0.10f));
	g.fillPath(stick, juce::AffineTransform::rotation(toAngle + 3.12f).translated(bounds.getCentre()));
	g.drawEllipse(bounds.withSizeKeepingCentre(radius * 1.25f, radius * 1.25f), lineW);
}

// This method defines the way a label is drawn.
void CustomLNF::drawLabel(juce::Graphics& g, juce::Label& label)
{
	LookAndFeel_V4::drawLabel(g, label);

	int fontSize = std::round(label.getLocalBounds().toFloat().getHeight() * 0.7f);

	label.setFont(juce::Font("Segoe UI", fontSize, juce::Font::plain));

}

// This method defines the way a linear slider is drawn.
void CustomLNF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                 float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle slider_style, juce::Slider& slider)
{
	auto bounds = slider.getLocalBounds();


	auto rectSliderValue = bounds.withTrimmedTop(juce::jmap(static_cast<int>(sliderPos), static_cast<int>(y), static_cast<int>(height), 0, bounds.getHeight()));

	float alphaDark = 0.1f;
	float alphaBright = 0.9f;

	g.setColour(slider.findColour(juce::Slider::trackColourId));
	g.fillRoundedRectangle(rectSliderValue.toFloat(), 5.0f);

	juce::ColourGradient gradient(slider.findColour(juce::Slider::trackColourId).brighter(0.3f).withAlpha(alphaBright), rectSliderValue.getX(), rectSliderValue.getCentre().getY() - 50.0f,
		slider.findColour(juce::Slider::trackColourId).darker(0.3f).withAlpha(alphaDark), rectSliderValue.getRight(), rectSliderValue.getCentre().getY() + 50.0f, true);
	g.setGradientFill(gradient);
	g.fillRoundedRectangle(rectSliderValue.toFloat(), 5.0f);

	g.setColour(slider.findColour(juce::Slider::trackColourId).brighter(0.3f));
	g.drawRoundedRectangle(rectSliderValue.toFloat(), 5.0f, 2.0f);

	g.setColour(ThemeColorManager::mdmTextWhite.darker(0.10f));
	g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}


// This method defines the way a button background is drawn.
void CustomLNF::drawButtonBackground(juce::Graphics& g, juce::Button& button,
	const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto bounds = button.getLocalBounds();

	g.setColour(ThemeColorManager::mdmTrioDark.brighter(0.05f));
	g.fillRoundedRectangle(bounds.toFloat(), 10.0f);

	float alphaDark, alphaBright;

	if (button.getToggleState()) // button is activated
	{
		alphaDark = 0.5f;
		alphaBright = 1.0f;

		g.setColour(button.findColour(juce::TextButton::buttonColourId));
		g.fillRoundedRectangle(bounds.toFloat(), 10.0f);

		juce::ColourGradient gradient(button.findColour(juce::TextButton::buttonColourId).brighter(0.3f).withAlpha(alphaBright), bounds.getX(), bounds.getCentre().getY() - 50.0f,
			button.findColour(juce::TextButton::buttonColourId).darker(0.3f).withAlpha(alphaDark), bounds.getRight(), bounds.getCentre().getY() + 20.0f, true);
		g.setGradientFill(gradient);
		g.fillRoundedRectangle(bounds.toFloat(), 10.0f);
	}

	// Defining and painting two gradients which are combined to create a background for the button.
	juce::ColourGradient backgroundGradient(button.findColour(juce::TextButton::buttonColourId).withAlpha(1.0f).withBrightness(5.0f).withSaturation(1.5f), bounds.getCentreX(), bounds.getBottom() + 100.0f ,
	    ThemeColorManager::mdmTrioDark.brighter(0.4f).withAlpha(0.0f), bounds.getCentreX(), bounds.getY() - 20.0f, true);
	g.setGradientFill(backgroundGradient);
	g.fillRoundedRectangle(bounds.toFloat(), 10.0f);


	juce::ColourGradient gradientWhite(juce::Colours::white.withAlpha(0.05f), bounds.getCentreX() - 10.0f, bounds.getY() - 20.0f,
		ThemeColorManager::mdmTrioDark.withAlpha(0.0f), bounds.getCentreX(), bounds.getBottom() + 10.0f, true);
	g.setGradientFill(gradientWhite);
	g.fillRoundedRectangle(bounds.toFloat(), 10.0f);

	g.setColour(ThemeColorManager::mdmTextWhite.darker(0.5f));
	g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2);
}

// This method defines the way a combo box is drawn.
void CustomLNF::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX,
	int buttonY, int buttonW, int buttonH, juce::ComboBox& combo_box)
{
	auto bounds = combo_box.getLocalBounds();

	g.setColour(ThemeColorManager::mdmTrioDark.brighter(0.05f));
	g.fillRoundedRectangle(bounds.toFloat(), 10.0f);

	// Defining and painting two gradients which are combined to create a background for the button.
	juce::ColourGradient backgroundGradient(combo_box.findColour(juce::ComboBox::backgroundColourId).withAlpha(1.0f).withBrightness(5.0f).withSaturation(1.5f), bounds.getCentreX(), bounds.getBottom() + 100.0f,
		ThemeColorManager::mdmTrioDark.brighter(0.4f).withAlpha(0.0f), bounds.getCentreX(), bounds.getY() - 20.0f, true);
	g.setGradientFill(backgroundGradient);
	g.fillRoundedRectangle(bounds.toFloat(), 10.0f);


	juce::ColourGradient gradientWhite(juce::Colours::white.withAlpha(0.05f), bounds.getCentreX() - 10.0f, bounds.getY() - 20.0f,
		ThemeColorManager::mdmTrioDark.withAlpha(0.0f), bounds.getCentreX(), bounds.getBottom() + 10.0f, true);
	g.setGradientFill(gradientWhite);
	g.fillRoundedRectangle(bounds.toFloat(), 10.0f);

	g.setColour(ThemeColorManager::mdmTextWhite.darker(0.5f));
	g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2);
}

// This method defines the way a popup menu background is drawn.
void CustomLNF::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
{
	g.fillAll(ThemeColorManager::mdmTrioDark);
	g.setColour(ThemeColorManager::mdmTextWhite);
	g.drawRect(0, 0, width, height, 1);
}

// This method defines the way a toggle button is drawn.
void CustomLNF::drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggle_button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto bounds = toggle_button.getLocalBounds();
	int height = bounds.getHeight();
	float cornerSize = height * 0.25f;
	auto rectangleBounds = bounds.withSizeKeepingCentre(height, height);
	int lineWidth = 1;

	if (toggle_button.getToggleState()) // button is activated
	{
		g.setColour(ThemeColorManager::mdmTextWhite.brighter(0.7f));
		g.fillRoundedRectangle(rectangleBounds.toFloat(), cornerSize );
	}
	else
	{
		g.setColour(ThemeColorManager::mdmTrioDark);
		g.fillRoundedRectangle(rectangleBounds.toFloat(), cornerSize);
	}

	g.setColour(ThemeColorManager::mdmTextWhite.darker(0.5f));
	g.drawRoundedRectangle(rectangleBounds.toFloat(), cornerSize, lineWidth);
}

// This method changes the slider text box settings.
CustomLNF::CustomLabel* CustomLNF::createSliderTextBox(juce::Slider& slider)
{
	auto* label = new CustomLabel();
	label->setJustificationType(juce::Justification::centred);
	label->setColour(juce::Label::textColourId, ThemeColorManager::mdmTextWhite);
	label->setColour(juce::Label::textWhenEditingColourId, slider.findColour(juce::Slider::textBoxTextColourId));
	label->setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
	label->setInterceptsMouseClicks(false, false);
	label->setFont(juce::Font("Segoe UI", 16, juce::Font::plain));
	return label;
}

// Changing the color used for the caret.
juce::CaretComponent* CustomLNF::createCaretComponent(juce::Component* keyFocusOwner)
{
	auto caret = new juce::CaretComponent(keyFocusOwner);
	caret->setColour(juce::CaretComponent::caretColourId, ThemeColorManager::mdmTextWhite);
	return caret;
}