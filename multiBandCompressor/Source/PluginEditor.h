/*
  ==============================================================================

    This file contains the custom JUCE editor for SpectrumMixer.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SpectrumLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SpectrumLookAndFeel();

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider&) override;

    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle, juce::Slider&) override;

    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;
};

class LevelMeter : public juce::Component
{
public:
    void setColours (juce::Colour mainColour, juce::Colour topColour);
    void setLevel (float newLevel);
    void paint (juce::Graphics&) override;

private:
    juce::Colour baseColour { juce::Colour (0xff22d3ee) };
    juce::Colour peakColour { juce::Colour (0xff67e8f9) };
    float level = 0.55f;
};

class AnalyzerDisplay : public juce::Component
{
public:
    void setPhase (float newPhase);
    void paint (juce::Graphics&) override;

private:
    float phase = 0.0f;
};

class TrackStrip : public juce::Component
{
public:
    TrackStrip();
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::StringArray names { "KICK", "SNARE", "PAD", "LEAD" };
    juce::Array<juce::Colour> colours {
        juce::Colour (0xff22d3ee),
        juce::Colour (0xffa78bfa),
        juce::Colour (0xfffb923c),
        juce::Colour (0xff34d399)
    };
};

class MultiBandCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                 public juce::Slider::Listener,
                                                 private juce::Timer
{
public:
    MultiBandCompressorAudioProcessorEditor (MultiBandCompressorAudioProcessor&);
    ~MultiBandCompressorAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    void configureKnob (juce::Slider&, juce::Label&, const juce::String& labelText,
                        const juce::String& suffix);
    void configureButton (juce::TextButton&, const juce::String& text);
    void layoutBand (juce::Rectangle<int>, const juce::String& title, juce::Colour colour,
                     LevelMeter&, std::array<juce::Slider*, 4>, std::array<juce::Label*, 4>);
    void drawPanel (juce::Graphics&, juce::Rectangle<float>, juce::Colour edge,
                    float cornerSize = 8.0f);
    void timerCallback() override;

    MultiBandCompressorAudioProcessor& audioProcessor;
    SpectrumLookAndFeel spectrumLookAndFeel;

    AnalyzerDisplay analyzer;
    TrackStrip trackStrip;
    LevelMeter lowMeter, midMeter, highMeter, outLeftMeter, outRightMeter;

    juce::Slider crossoverSlider;
    juce::Label lowCrossoverLabel;
    juce::Label highCrossoverLabel;

    juce::Slider lowThreshold, lowAttack, lowRelease, lowRatio;
    juce::Slider midThreshold, midAttack, midRelease, midRatio;
    juce::Slider highThreshold, highAttack, highRelease, highRatio;

    juce::Label lowThresholdLabel, lowAttackLabel, lowReleaseLabel, lowRatioLabel;
    juce::Label midThresholdLabel, midAttackLabel, midReleaseLabel, midRatioLabel;
    juce::Label highThresholdLabel, highAttackLabel, highReleaseLabel, highRatioLabel;

    juce::TextButton previousPresetButton { "<" };
    juce::TextButton nextPresetButton { ">" };
    juce::TextButton simpleButton { "SIMPLE" };
    juce::TextButton abButton { "A / B" };
    juce::TextButton bypassButton { "BYPASS" };
    juce::TextButton preButton { "PRE" };
    juce::TextButton postButton { "POST" };
    juce::TextButton freezeButton { "FREEZE" };
    juce::TextButton analyzeButton { "ANALYZE TRACK" };

    std::unique_ptr<SliderAttachment> lowThresholdAttachment;
    std::unique_ptr<SliderAttachment> lowAttackAttachment;
    std::unique_ptr<SliderAttachment> lowReleaseAttachment;
    std::unique_ptr<SliderAttachment> lowRatioAttachment;
    std::unique_ptr<SliderAttachment> midThresholdAttachment;
    std::unique_ptr<SliderAttachment> midAttackAttachment;
    std::unique_ptr<SliderAttachment> midReleaseAttachment;
    std::unique_ptr<SliderAttachment> midRatioAttachment;
    std::unique_ptr<SliderAttachment> highThresholdAttachment;
    std::unique_ptr<SliderAttachment> highAttackAttachment;
    std::unique_ptr<SliderAttachment> highReleaseAttachment;
    std::unique_ptr<SliderAttachment> highRatioAttachment;

    juce::Random random;
    float animationPhase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiBandCompressorAudioProcessorEditor)
};
