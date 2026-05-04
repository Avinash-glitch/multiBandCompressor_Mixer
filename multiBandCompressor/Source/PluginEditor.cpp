/*
  ==============================================================================

    This file contains the custom JUCE editor for SpectrumMixer.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    constexpr auto background = 0xff0a0d12;
    constexpr auto panel      = 0xff11161d;
    constexpr auto panelDark  = 0xff0c1015;
    constexpr auto line       = 0xff1f2733;
    constexpr auto lineBright = 0xff2a3445;
    constexpr auto ink        = 0xffe8edf5;
    constexpr auto sub        = 0xff8a93a3;
    constexpr auto dim        = 0xff5a6273;
    constexpr auto low        = 0xff22d3ee;
    constexpr auto mid        = 0xffa78bfa;
    constexpr auto high       = 0xfffb923c;
    constexpr auto good       = 0xff34d399;

    juce::Font displayFont (float size, juce::Font::FontStyleFlags style = juce::Font::plain)
    {
        return juce::Font (juce::FontOptions ("Orbitron", size, style)).withExtraKerningFactor (0.08f);
    }

    juce::Font monoFont (float size, juce::Font::FontStyleFlags style = juce::Font::plain)
    {
        return juce::Font (juce::FontOptions ("Menlo", size, style));
    }

    void drawLed (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour, bool on = true)
    {
        g.setColour (juce::Colour (0xff05070a));
        g.fillEllipse (area.expanded (1.0f));

        auto ledColour = on ? colour : juce::Colour (0xff1f2937);
        g.setColour (ledColour.withAlpha (on ? 0.32f : 0.0f));
        g.fillEllipse (area.expanded (5.0f));
        g.setColour (ledColour);
        g.fillEllipse (area);
    }
}

SpectrumLookAndFeel::SpectrumLookAndFeel()
{
    setColour (juce::Slider::textBoxTextColourId, juce::Colour (ink));
    setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff0a0e14));
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (line));
    setColour (juce::Label::textColourId, juce::Colour (sub));
    setColour (juce::TextButton::textColourOffId, juce::Colour (sub));
    setColour (juce::TextButton::textColourOnId, juce::Colour (background));
}

void SpectrumLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                            juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (5.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto accent = slider.findColour (juce::Slider::rotarySliderFillColourId);

    g.setColour (juce::Colour (0xff05070a));
    g.fillEllipse (bounds.expanded (3.0f));

    juce::Path arc;
    arc.addCentredArc (centre.x, centre.y, radius + 4.0f, radius + 4.0f, 0.0f,
                       rotaryStartAngle, angle, true);
    g.setColour (accent.withAlpha (0.85f));
    g.strokePath (arc, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved,
                                            juce::PathStrokeType::rounded));

    juce::ColourGradient body (juce::Colour (0xff2b3340), centre.x - radius * 0.35f, centre.y - radius * 0.45f,
                               juce::Colour (0xff080b0f), centre.x + radius * 0.4f, centre.y + radius * 0.55f,
                               true);
    body.addColour (0.55, juce::Colour (0xff14181f));
    g.setGradientFill (body);
    g.fillEllipse (bounds);

    g.setColour (juce::Colour (lineBright));
    g.drawEllipse (bounds, 1.0f);

    auto pointerLength = radius * 0.55f;
    auto pointerThickness = 2.5f;
    juce::Path pointer;
    pointer.addRoundedRectangle (-pointerThickness * 0.5f, -radius + 8.0f,
                                 pointerThickness, pointerLength, 2.0f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
    g.setColour (accent);
    g.fillPath (pointer);
}

void SpectrumLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto accent = slider.findColour (juce::Slider::trackColourId);

    if (style == juce::Slider::TwoValueHorizontal)
    {
        auto track = juce::Rectangle<float> ((float) x + 8.0f, (float) y + height * 0.5f - 3.0f,
                                            (float) width - 16.0f, 6.0f);
        g.setColour (juce::Colour (0xff070a0e));
        g.fillRoundedRectangle (track.expanded (1.0f), 4.0f);
        g.setColour (juce::Colour (line));
        g.drawRoundedRectangle (track.expanded (1.0f), 4.0f, 1.0f);

        auto selected = juce::Rectangle<float> (minSliderPos, track.getY(),
                                               maxSliderPos - minSliderPos, track.getHeight());
        juce::ColourGradient fill (juce::Colour (low), selected.getX(), selected.getCentreY(),
                                   juce::Colour (high), selected.getRight(), selected.getCentreY(), false);
        fill.addColour (0.52, juce::Colour (mid));
        g.setGradientFill (fill);
        g.fillRoundedRectangle (selected, 4.0f);

        for (auto thumbX : { minSliderPos, maxSliderPos })
        {
            g.setColour (juce::Colour (0xffe6ebf3));
            g.fillRoundedRectangle (thumbX - 5.0f, track.getCentreY() - 11.0f, 10.0f, 22.0f, 3.0f);
            g.setColour (juce::Colour (0xff05070a));
            g.drawRoundedRectangle (thumbX - 5.0f, track.getCentreY() - 11.0f, 10.0f, 22.0f, 3.0f, 1.0f);
        }
        return;
    }

    juce::LookAndFeel_V4::drawLinearSlider (g, x, y, width, height, sliderPos, minSliderPos,
                                            maxSliderPos, style, slider);
    juce::ignoreUnused (accent);
}

void SpectrumLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                                const juce::Colour&, bool highlighted, bool down)
{
    auto b = button.getLocalBounds().toFloat().reduced (0.5f);
    auto isOn = button.getToggleState();
    auto accent = button.findColour (juce::TextButton::buttonOnColourId);
    auto top = isOn ? accent.brighter (0.25f) : juce::Colour (0xff161c25);
    auto bottom = isOn ? accent.darker (0.25f) : juce::Colour (0xff10141b);

    if (highlighted || down)
    {
        top = top.brighter (0.08f);
        bottom = bottom.brighter (0.04f);
    }

    juce::ColourGradient fill (top, b.getCentreX(), b.getY(), bottom, b.getCentreX(), b.getBottom(), false);
    g.setGradientFill (fill);
    g.fillRoundedRectangle (b, 5.0f);

    g.setColour ((isOn ? accent : juce::Colour (lineBright)).withAlpha (0.9f));
    g.drawRoundedRectangle (b, 5.0f, 1.0f);
}

void LevelMeter::setColours (juce::Colour mainColour, juce::Colour topColour)
{
    baseColour = mainColour;
    peakColour = topColour;
    repaint();
}

void LevelMeter::setLevel (float newLevel)
{
    level = juce::jlimit (0.0f, 1.0f, newLevel);
    repaint();
}

void LevelMeter::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat().reduced (5.0f);
    g.setColour (juce::Colour (0xff070a0e));
    g.fillRoundedRectangle (area, 6.0f);
    g.setColour (juce::Colour (line));
    g.drawRoundedRectangle (area, 6.0f, 1.0f);

    for (int i = 0; i < 9; ++i)
    {
        auto y = area.getY() + (float) i * area.getHeight() / 8.0f;
        g.setColour (juce::Colour (0xffffffff).withAlpha (0.035f));
        g.drawHorizontalLine ((int) y, area.getX() + 7.0f, area.getRight() - 7.0f);
    }

    auto fill = area.reduced (6.0f);
    fill.removeFromTop (fill.getHeight() * (1.0f - level));
    juce::ColourGradient gradient (peakColour, fill.getCentreX(), fill.getY(),
                                   baseColour.darker (0.45f), fill.getCentreX(), fill.getBottom(), false);
    g.setGradientFill (gradient);
    g.fillRoundedRectangle (fill, 4.0f);

    auto peakY = fill.getY() - 3.0f;
    g.setColour (juce::Colours::white.withAlpha (0.8f));
    g.fillRoundedRectangle (area.getX() + 8.0f, peakY, area.getWidth() - 16.0f, 2.0f, 1.0f);
}

void AnalyzerDisplay::setPhase (float newPhase)
{
    phase = newPhase;
    repaint();
}

void AnalyzerDisplay::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    juce::ColourGradient bg (juce::Colour (0xff0d1117), area.getCentreX(), area.getY(),
                             juce::Colour (0xff070a0e), area.getCentreX(), area.getBottom(), false);
    g.setGradientFill (bg);
    g.fillRoundedRectangle (area, 8.0f);

    g.setColour (juce::Colour (line));
    for (int x = 0; x < getWidth(); x += 44)
        g.drawVerticalLine (x, area.getY(), area.getBottom());
    for (int y = 0; y < getHeight(); y += 24)
        g.drawHorizontalLine (y, area.getX(), area.getRight());

    auto drawBand = [&] (juce::Colour colour, float offset, float amplitude, float baseline)
    {
        juce::Path p;
        for (int i = 0; i < getWidth(); ++i)
        {
            auto x = (float) i;
            auto norm = x / juce::jmax (1.0f, (float) getWidth());
            auto envelope = std::sin (juce::MathConstants<float>::pi * norm);
            auto y = area.getY() + area.getHeight() * baseline
                   - std::sin ((norm * 9.0f + phase + offset) * juce::MathConstants<float>::twoPi)
                         * amplitude * envelope
                   - std::sin ((norm * 27.0f + phase * 0.4f + offset) * juce::MathConstants<float>::twoPi)
                         * amplitude * 0.2f;
            if (i == 0) p.startNewSubPath (x, y);
            else        p.lineTo (x, y);
        }

        g.setColour (colour.withAlpha (0.42f));
        g.strokePath (p, juce::PathStrokeType (2.2f));
    };

    drawBand (juce::Colour (low), 0.02f, 28.0f, 0.55f);
    drawBand (juce::Colour (mid), 0.22f, 20.0f, 0.47f);
    drawBand (juce::Colour (high), 0.44f, 14.0f, 0.38f);
}

TrackStrip::TrackStrip()
{
    setInterceptsMouseClicks (false, false);
}

void TrackStrip::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour (juce::Colour (panelDark));
    g.fillRoundedRectangle (area, 8.0f);
    g.setColour (juce::Colour (line));
    g.drawRoundedRectangle (area, 8.0f, 1.0f);

    auto rowArea = getLocalBounds().reduced (10, 12);
    auto rowHeight = rowArea.getHeight() / names.size();

    for (int i = 0; i < names.size(); ++i)
    {
        auto row = rowArea.removeFromTop (rowHeight).reduced (0, 4).toFloat();
        auto active = i == 0;
        auto colour = colours.getReference (i);

        g.setColour (active ? colour.withAlpha (0.14f) : juce::Colour (0xff121821));
        g.fillRoundedRectangle (row, 7.0f);
        g.setColour (active ? colour.withAlpha (0.75f) : juce::Colour (line));
        g.drawRoundedRectangle (row, 7.0f, 1.0f);

        drawLed (g, row.withWidth (7.0f).withHeight (7.0f).withCentre ({ row.getX() + 15.0f, row.getCentreY() }),
                 colour, active);

        g.setFont (displayFont (12.0f, juce::Font::bold));
        g.setColour (active ? colour : juce::Colour (ink));
        g.drawText (names[i], row.reduced (28.0f, 0.0f), juce::Justification::centredLeft);

        g.setFont (monoFont (10.0f));
        g.setColour (juce::Colour (sub));
        g.drawText (i == 0 ? "M  S" : "M  S", row.removeFromRight (44.0f), juce::Justification::centred);
    }
}

void TrackStrip::resized() {}

MultiBandCompressorAudioProcessorEditor::MultiBandCompressorAudioProcessorEditor (MultiBandCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&spectrumLookAndFeel);
    setOpaque (true);

    lowMeter.setColours (juce::Colour (low), juce::Colour (0xff67e8f9));
    midMeter.setColours (juce::Colour (mid), juce::Colour (0xffc4b5fd));
    highMeter.setColours (juce::Colour (high), juce::Colour (0xffffd2a8));
    outLeftMeter.setColours (juce::Colour (good), juce::Colour (0xfffacc15));
    outRightMeter.setColours (juce::Colour (good), juce::Colour (0xfffacc15));

    for (auto* c : { static_cast<juce::Component*> (&analyzer),
                     static_cast<juce::Component*> (&trackStrip),
                     static_cast<juce::Component*> (&lowMeter),
                     static_cast<juce::Component*> (&midMeter),
                     static_cast<juce::Component*> (&highMeter),
                     static_cast<juce::Component*> (&outLeftMeter),
                     static_cast<juce::Component*> (&outRightMeter) })
        addAndMakeVisible (c);

    configureKnob (lowThreshold, lowThresholdLabel, "THRESH", " dB");
    configureKnob (lowAttack, lowAttackLabel, "ATTACK", " ms");
    configureKnob (lowRelease, lowReleaseLabel, "RELEASE", " ms");
    configureKnob (lowRatio, lowRatioLabel, "RATIO", ":1");

    configureKnob (midThreshold, midThresholdLabel, "THRESH", " dB");
    configureKnob (midAttack, midAttackLabel, "ATTACK", " ms");
    configureKnob (midRelease, midReleaseLabel, "RELEASE", " ms");
    configureKnob (midRatio, midRatioLabel, "RATIO", ":1");

    configureKnob (highThreshold, highThresholdLabel, "THRESH", " dB");
    configureKnob (highAttack, highAttackLabel, "ATTACK", " ms");
    configureKnob (highRelease, highReleaseLabel, "RELEASE", " ms");
    configureKnob (highRatio, highRatioLabel, "RATIO", ":1");

    for (auto* s : { &lowThreshold, &lowAttack, &lowRelease, &lowRatio })
        s->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (low));
    for (auto* s : { &midThreshold, &midAttack, &midRelease, &midRatio })
        s->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (mid));
    for (auto* s : { &highThreshold, &highAttack, &highRelease, &highRatio })
        s->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (high));

    lowThresholdAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "low_threshold", lowThreshold);
    lowAttackAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "low_attack", lowAttack);
    lowReleaseAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "low_release", lowRelease);
    lowRatioAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "low_ratio", lowRatio);
    midThresholdAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "mid_threshold", midThreshold);
    midAttackAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "mid_attack", midAttack);
    midReleaseAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "mid_release", midRelease);
    midRatioAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "mid_ratio", midRatio);
    highThresholdAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "high_threshold", highThreshold);
    highAttackAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "high_attack", highAttack);
    highReleaseAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "high_release", highRelease);
    highRatioAttachment = std::make_unique<SliderAttachment> (audioProcessor.getParameters(), "high_ratio", highRatio);

    crossoverSlider.setSliderStyle (juce::Slider::TwoValueHorizontal);
    crossoverSlider.setRange (20.0, 20000.0, 1.0);
    crossoverSlider.setSkewFactorFromMidPoint (1000.0);
    crossoverSlider.setMinValue (250.0, juce::dontSendNotification);
    crossoverSlider.setMaxValue (4000.0, juce::dontSendNotification);
    crossoverSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    crossoverSlider.setColour (juce::Slider::trackColourId, juce::Colour (low));
    crossoverSlider.addListener (this);
    addAndMakeVisible (crossoverSlider);

    lowCrossoverLabel.setText ("L/M 250 Hz", juce::dontSendNotification);
    highCrossoverLabel.setText ("M/H 4.0 kHz", juce::dontSendNotification);
    for (auto* label : { &lowCrossoverLabel, &highCrossoverLabel })
    {
        label->setFont (monoFont (12.0f));
        label->setColour (juce::Label::textColourId, juce::Colour (sub));
        label->setJustificationType (juce::Justification::centred);
        addAndMakeVisible (*label);
    }

    for (auto* button : { &previousPresetButton, &nextPresetButton, &simpleButton, &abButton,
                          &bypassButton, &preButton, &postButton, &freezeButton, &analyzeButton })
    {
        configureButton (*button, button->getButtonText());
        addAndMakeVisible (*button);
    }

    postButton.setToggleState (true, juce::dontSendNotification);
    postButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (low));
    analyzeButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (mid));
    analyzeButton.setToggleState (true, juce::dontSendNotification);

    setResizable (true, true);
    setResizeLimits (840, 560, 1400, 920);
    setSize (1120, 700);
    startTimerHz (24);
}

MultiBandCompressorAudioProcessorEditor::~MultiBandCompressorAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void MultiBandCompressorAudioProcessorEditor::configureKnob (juce::Slider& slider, juce::Label& label,
                                                             const juce::String& labelText,
                                                             const juce::String& suffix)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 64, 18);
    slider.setTextValueSuffix (suffix);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                                juce::MathConstants<float>::pi * 2.75f, true);
    addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setFont (monoFont (9.5f, juce::Font::bold));
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, juce::Colour (dim));
    addAndMakeVisible (label);
}

void MultiBandCompressorAudioProcessorEditor::configureButton (juce::TextButton& button,
                                                               const juce::String& text)
{
    button.setButtonText (text);
    button.setColour (juce::TextButton::buttonOnColourId, juce::Colour (low));
    button.setColour (juce::TextButton::textColourOffId, juce::Colour (sub));
    button.setColour (juce::TextButton::textColourOnId, juce::Colour (background));
}

void MultiBandCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (background));

    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient wash (juce::Colour (low).withAlpha (0.09f), bounds.getX(), bounds.getY(),
                               juce::Colour (mid).withAlpha (0.06f), bounds.getRight(), bounds.getY(), false);
    wash.addColour (0.6, juce::Colour (background).withAlpha (0.0f));
    g.setGradientFill (wash);
    g.fillRect (bounds.withHeight (190.0f));

    auto header = getLocalBounds().removeFromTop (68).toFloat();
    g.setColour (juce::Colour (0xff151b24));
    g.fillRect (header);
    for (int x = 0; x < getWidth(); x += 4)
    {
        g.setColour (juce::Colours::white.withAlpha (0.018f));
        g.drawVerticalLine (x, header.getY(), header.getBottom());
    }
    g.setColour (juce::Colour (line));
    g.drawLine (juce::Line<float> (header.getBottomLeft(), header.getBottomRight()), 1.0f);

    drawLed (g, { 24.0f, 27.0f, 9.0f, 9.0f }, juce::Colour (low));
    g.setFont (displayFont (18.0f, juce::Font::bold));
    g.setColour (juce::Colour (ink));
    g.drawText ("SPECTRUM", 44, 18, 116, 24, juce::Justification::centredLeft);
    g.setColour (juce::Colour (low));
    g.drawText ("MIXER", 158, 18, 88, 24, juce::Justification::centredLeft);

    g.setFont (monoFont (10.0f));
    g.setColour (juce::Colour (dim));
    g.drawText ("MULTIBAND COMPRESSOR  /  v2 JUCE", 45, 40, 260, 18, juce::Justification::centredLeft);
    g.drawText ("Modern Master / Punchy", getWidth() / 2 - 92, 23, 184, 18, juce::Justification::centred);

    auto area = getLocalBounds().reduced (18);
    area.removeFromTop (72);
    auto side = area.removeFromLeft (226);
    area.removeFromLeft (16);

    drawPanel (g, side.removeFromTop (286).toFloat(), juce::Colour (line));
    side.removeFromTop (16);
    drawPanel (g, side.removeFromTop (184).toFloat(), juce::Colour (line));

    auto analyzerPanel = area.removeFromTop (204);
    drawPanel (g, analyzerPanel.toFloat(), juce::Colour (line));
    area.removeFromTop (16);

    auto bandArea = area.removeFromTop (300);
    auto bandWidth = (bandArea.getWidth() - 24) / 3;
    auto lowBandPanel = bandArea.removeFromLeft (bandWidth).toFloat();
    bandArea.removeFromLeft (12);
    auto midBandPanel = bandArea.removeFromLeft (bandWidth).toFloat();
    bandArea.removeFromLeft (12);
    auto highBandPanel = bandArea.removeFromLeft (bandWidth).toFloat();

    drawPanel (g, lowBandPanel, juce::Colour (low).withAlpha (0.7f));
    drawPanel (g, midBandPanel, juce::Colour (mid).withAlpha (0.7f));
    drawPanel (g, highBandPanel, juce::Colour (high).withAlpha (0.7f));

    auto drawBandTitle = [&] (juce::Rectangle<float> panelArea, const juce::String& title,
                              const juce::String& range, juce::Colour colour)
    {
        auto headerArea = panelArea.reduced (16.0f, 10.0f).withHeight (26.0f);
        drawLed (g, headerArea.withWidth (7.0f).withHeight (7.0f)
                          .withCentre ({ headerArea.getX() + 4.0f, headerArea.getCentreY() }),
                 colour);

        g.setFont (displayFont (12.0f, juce::Font::bold));
        g.setColour (colour);
        g.drawText (title, headerArea.withTrimmedLeft (16.0f).withWidth (80.0f),
                    juce::Justification::centredLeft);

        g.setFont (monoFont (10.0f));
        g.setColour (juce::Colour (dim));
        g.drawText (range, headerArea.removeFromRight (112.0f), juce::Justification::centredRight);
    };

    drawBandTitle (lowBandPanel, "LOW", "20 Hz - 250 Hz", juce::Colour (low));
    drawBandTitle (midBandPanel, "MID", "250 Hz - 4 kHz", juce::Colour (mid));
    drawBandTitle (highBandPanel, "HIGH", "4 kHz - 20 kHz", juce::Colour (high));

    g.setFont (displayFont (11.0f, juce::Font::bold));
    g.setColour (juce::Colour (sub));
    g.drawText ("TRACKS", 36, 94, 160, 18, juce::Justification::centredLeft);
    g.drawText ("OUTPUT", 36, 394, 160, 18, juce::Justification::centredLeft);
    g.drawText ("MULTIBAND ANALYZER", 268, 94, 250, 18, juce::Justification::centredLeft);
    g.drawText ("CROSSOVER", 268, 252, 160, 18, juce::Justification::centredLeft);
    g.drawText ("AI MIX ASSIST", 268, getHeight() - 74, 160, 18, juce::Justification::centredLeft);

    g.setFont (monoFont (10.0f));
    g.setColour (juce::Colour (dim));
    g.drawText ("CPU 12%  /  LATENCY 4.2 ms  /  48 kHz", 268, getHeight() - 42, 350, 18,
                juce::Justification::centredLeft);
}

void MultiBandCompressorAudioProcessorEditor::drawPanel (juce::Graphics& g,
                                                         juce::Rectangle<float> area,
                                                         juce::Colour edge,
                                                         float cornerSize)
{
    juce::ColourGradient fill (juce::Colour (panel), area.getCentreX(), area.getY(),
                               juce::Colour (panelDark), area.getCentreX(), area.getBottom(), false);
    g.setGradientFill (fill);
    g.fillRoundedRectangle (area, cornerSize);
    g.setColour (edge.withAlpha (0.85f));
    g.drawRoundedRectangle (area, cornerSize, 1.0f);
}

void MultiBandCompressorAudioProcessorEditor::resized()
{
    auto header = getLocalBounds().removeFromTop (68).reduced (18, 0);
    auto preset = header.withSizeKeepingCentre (276, 36);
    previousPresetButton.setBounds (preset.removeFromLeft (34).reduced (3));
    nextPresetButton.setBounds (preset.removeFromRight (34).reduced (3));

    auto controls = header.removeFromRight (318).withSizeKeepingCentre (318, 32);
    simpleButton.setBounds (controls.removeFromLeft (76).reduced (4, 3));
    abButton.setBounds (controls.removeFromLeft (70).reduced (4, 3));
    bypassButton.setBounds (controls.removeFromLeft (92).reduced (4, 3));

    auto area = getLocalBounds().reduced (18);
    area.removeFromTop (72);

    auto side = area.removeFromLeft (226);
    area.removeFromLeft (16);
    auto tracksPanel = side.removeFromTop (286).reduced (12, 34);
    trackStrip.setBounds (tracksPanel);

    side.removeFromTop (16);
    auto outputPanel = side.removeFromTop (184).reduced (16, 44);
    auto outputMeterWidth = (outputPanel.getWidth() - 10) / 2;
    outLeftMeter.setBounds (outputPanel.removeFromLeft (outputMeterWidth));
    outputPanel.removeFromLeft (10);
    outRightMeter.setBounds (outputPanel.removeFromLeft (outputMeterWidth));

    auto analyzerPanel = area.removeFromTop (204);
    auto analyzerHeader = analyzerPanel.removeFromTop (42).reduced (14, 7);
    preButton.setBounds (analyzerHeader.removeFromRight (58).reduced (4, 2));
    postButton.setBounds (analyzerHeader.removeFromRight (64).reduced (4, 2));
    freezeButton.setBounds (analyzerHeader.removeFromRight (78).reduced (4, 2));
    analyzer.setBounds (analyzerPanel.reduced (16, 8));

    auto xoverStrip = analyzer.getBounds().removeFromBottom (44).reduced (18, 5);
    lowCrossoverLabel.setBounds (xoverStrip.removeFromLeft (112));
    highCrossoverLabel.setBounds (xoverStrip.removeFromRight (112));
    crossoverSlider.setBounds (xoverStrip.reduced (10, 2));

    area.removeFromTop (16);

    auto bandArea = area.removeFromTop (300);
    auto bandWidth = (bandArea.getWidth() - 24) / 3;
    auto lowBand = bandArea.removeFromLeft (bandWidth);
    bandArea.removeFromLeft (12);
    auto midBand = bandArea.removeFromLeft (bandWidth);
    bandArea.removeFromLeft (12);
    auto highBand = bandArea.removeFromLeft (bandWidth);

    layoutBand (lowBand, "LOW", juce::Colour (low), lowMeter,
                { &lowThreshold, &lowAttack, &lowRelease, &lowRatio },
                { &lowThresholdLabel, &lowAttackLabel, &lowReleaseLabel, &lowRatioLabel });
    layoutBand (midBand, "MID", juce::Colour (mid), midMeter,
                { &midThreshold, &midAttack, &midRelease, &midRatio },
                { &midThresholdLabel, &midAttackLabel, &midReleaseLabel, &midRatioLabel });
    layoutBand (highBand, "HIGH", juce::Colour (high), highMeter,
                { &highThreshold, &highAttack, &highRelease, &highRatio },
                { &highThresholdLabel, &highAttackLabel, &highReleaseLabel, &highRatioLabel });

    auto bottom = area.removeFromBottom (94).reduced (0, 14);
    analyzeButton.setBounds (bottom.removeFromLeft (190).reduced (0, 14));
}

void MultiBandCompressorAudioProcessorEditor::layoutBand (juce::Rectangle<int> band,
                                                          const juce::String& title,
                                                          juce::Colour colour,
                                                          LevelMeter& meter,
                                                          std::array<juce::Slider*, 4> sliders,
                                                          std::array<juce::Label*, 4> labels)
{
    juce::ignoreUnused (title, colour);

    auto inner = band.reduced (14);
    inner.removeFromTop (34);
    meter.setBounds (inner.removeFromTop (94));
    inner.removeFromTop (12);

    auto knobRow = inner.removeFromTop (120);
    auto knobWidth = knobRow.getWidth() / 4;

    for (size_t i = 0; i < sliders.size(); ++i)
    {
        auto cell = knobRow.removeFromLeft (knobWidth).reduced (2, 0);
        labels[i]->setBounds (cell.removeFromTop (16));
        sliders[i]->setBounds (cell);
    }
}

void MultiBandCompressorAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider != &crossoverSlider)
        return;

    auto lowFreq = crossoverSlider.getMinValue();
    auto highFreq = crossoverSlider.getMaxValue();

    if (auto* lowParam = audioProcessor.getParameters().getParameter ("low_mid_freq"))
        lowParam->setValueNotifyingHost (lowParam->convertTo0to1 ((float) lowFreq));

    if (auto* highParam = audioProcessor.getParameters().getParameter ("mid_high_freq"))
        highParam->setValueNotifyingHost (highParam->convertTo0to1 ((float) highFreq));

    lowCrossoverLabel.setText ("L/M " + juce::String (lowFreq, 0) + " Hz", juce::dontSendNotification);
    highCrossoverLabel.setText ("M/H " + juce::String (highFreq / 1000.0, 1) + " kHz", juce::dontSendNotification);
}

void MultiBandCompressorAudioProcessorEditor::timerCallback()
{
    animationPhase += 0.015f;
    analyzer.setPhase (animationPhase);

    auto wobble = [&] (float base, float depth)
    {
        return juce::jlimit (0.08f, 0.95f, base + random.nextFloat() * depth - depth * 0.5f);
    };

    lowMeter.setLevel (wobble (0.58f, 0.18f));
    midMeter.setLevel (wobble (0.46f, 0.16f));
    highMeter.setLevel (wobble (0.36f, 0.14f));
    outLeftMeter.setLevel (wobble (0.64f, 0.18f));
    outRightMeter.setLevel (wobble (0.61f, 0.18f));
}
