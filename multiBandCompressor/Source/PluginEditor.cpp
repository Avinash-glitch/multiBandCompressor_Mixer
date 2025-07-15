/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void MultiBandCompressorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &crossoverSlider)
    {
        // Update parameter values
        auto lowFreq = crossoverSlider.getMinValue();
        auto highFreq = crossoverSlider.getMaxValue();

        // Update your processor parameters
        audioProcessor.getParameters().getParameter("low_mid_freq")->setValueNotifyingHost(
            audioProcessor.getParameters().getParameter("low_mid_freq")->convertTo0to1(lowFreq));
        audioProcessor.getParameters().getParameter("mid_high_freq")->setValueNotifyingHost(
            audioProcessor.getParameters().getParameter("mid_high_freq")->convertTo0to1(highFreq));

        // Update label text
        lowCrossoverLabel.setText("Low: " + juce::String(lowFreq, 0) + " Hz", juce::dontSendNotification);
        highCrossoverLabel.setText("High: " + juce::String(highFreq, 0) + " Hz", juce::dontSendNotification);
    }
}
//==============================================================================
MultiBandCompressorAudioProcessorEditor::MultiBandCompressorAudioProcessorEditor (MultiBandCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // === LOW BAND ===
    lowThreshold.setSliderStyle(juce::Slider::LinearVertical);
    lowThreshold.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    lowThreshold.setRange(-60.0f, 0.0f, 0.1f); // In dB
    addAndMakeVisible(lowThreshold);

    lowAttack.setSliderStyle(juce::Slider::LinearVertical);
    lowAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    lowAttack.setRange(0.1f, 100.0f, 0.1f); // In ms
    addAndMakeVisible(lowAttack);

    lowRelease.setSliderStyle(juce::Slider::LinearVertical);
    lowRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    lowRelease.setRange(1.0f, 500.0f, 1.0f); // In ms
    addAndMakeVisible(lowRelease);

    lowRatio.setSliderStyle(juce::Slider::LinearVertical);
    lowRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    lowRatio.setRange(1.0f, 20.0f, 0.1f); // Compression ratio
    addAndMakeVisible(lowRatio);

    // === MID BAND ===
    midThreshold.setSliderStyle(juce::Slider::LinearVertical);
    midThreshold.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    midThreshold.setRange(-60.0f, 0.0f, 0.1f);
    addAndMakeVisible(midThreshold);

    midAttack.setSliderStyle(juce::Slider::LinearVertical);
    midAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    midAttack.setRange(0.1f, 100.0f, 0.1f);
    addAndMakeVisible(midAttack);

    midRelease.setSliderStyle(juce::Slider::LinearVertical);
    midRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    midRelease.setRange(1.0f, 500.0f, 1.0f);
    addAndMakeVisible(midRelease);

    midRatio.setSliderStyle(juce::Slider::LinearVertical);
    midRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    midRatio.setRange(1.0f, 20.0f, 0.1f);
    addAndMakeVisible(midRatio);

    // === HIGH BAND ===
    highThreshold.setSliderStyle(juce::Slider::LinearVertical);
    highThreshold.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    highThreshold.setRange(-60.0f, 0.0f, 0.1f);
    addAndMakeVisible(highThreshold);

    highAttack.setSliderStyle(juce::Slider::LinearVertical);
    highAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    highAttack.setRange(0.1f, 100.0f, 0.1f);
    addAndMakeVisible(highAttack);

    highRelease.setSliderStyle(juce::Slider::LinearVertical);
    highRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    highRelease.setRange(1.0f, 500.0f, 1.0f);
    addAndMakeVisible(highRelease);

    highRatio.setSliderStyle(juce::Slider::LinearVertical);
    highRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60,20);
    highRatio.setRange(1.0f, 20.0f, 0.1f);
    addAndMakeVisible(highRatio);

    // LOW BAND
    lowThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "low_threshold", lowThreshold);
    lowAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "low_attack", lowAttack);
    lowReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "low_release", lowRelease);
    lowRatioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "low_ratio", lowRatio);

    // MID BAND
    midThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "mid_threshold", midThreshold);
    midAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "mid_attack", midAttack);
    midReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "mid_release", midRelease);
    midRatioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "mid_ratio", midRatio);

    // HIGH BAND
    highThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "high_threshold", highThreshold);
    highAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "high_attack", highAttack);
    highReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "high_release", highRelease);
    highRatioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "high_ratio", highRatio);

    // Setup range slider
    crossoverSlider.setSliderStyle(juce::Slider::TwoValueHorizontal);
    crossoverSlider.setRange(20.0, 20000.0, 1.0); // Min, Max, Step
    crossoverSlider.setSkewFactorFromMidPoint(1000.0); // Logarithmic scale
    crossoverSlider.setMinValue(300.0);  // Low-mid crossover default
    crossoverSlider.setMaxValue(3000.0); // Mid-high crossover default
    crossoverSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(crossoverSlider);

    // Setup labels
    lowCrossoverLabel.setText("Low: 300 Hz", juce::dontSendNotification);
    lowCrossoverLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lowCrossoverLabel);

    highCrossoverLabel.setText("High: 3000 Hz", juce::dontSendNotification);
    highCrossoverLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(highCrossoverLabel);

    // Add listener to update labels when slider changes
    crossoverSlider.addListener(this);
	// Set the size of the editor
     setSize (600, 600);
}

MultiBandCompressorAudioProcessorEditor::~MultiBandCompressorAudioProcessorEditor()
{
}

//==============================================================================
void MultiBandCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MultiBandCompressorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    // Band sections at the top
    int sectionWidth = 4 * 40; // 4 sliders per band, each 70px wide
    int gap = 15;

    auto lowBandArea = area.removeFromLeft(sectionWidth);
    area.removeFromLeft(gap); // Remove gap space
    auto midBandArea = area.removeFromLeft(sectionWidth);
    area.removeFromLeft(gap); // Remove gap space  
    auto highBandArea = area.removeFromLeft(sectionWidth);

    // Lambda function for band layout
    auto layoutBand = [](juce::Rectangle<int> bandArea,
        juce::Slider& thresh, juce::Slider& att,
        juce::Slider& rel, juce::Slider& ratio)
        {
            auto w = bandArea.getWidth() / 4;
            int sliderHeight = 80; // Set exact height you want

            // Center the sliders vertically in the band area
            int yOffset = (bandArea.getHeight() - sliderHeight) / 2;

            thresh.setBounds(bandArea.removeFromLeft(w).removeFromTop(sliderHeight).translated(0, yOffset));
            att.setBounds(bandArea.removeFromLeft(w).removeFromTop(sliderHeight).translated(0, yOffset));
            rel.setBounds(bandArea.removeFromLeft(w).removeFromTop(sliderHeight).translated(0, yOffset));
            ratio.setBounds(bandArea.removeFromLeft(w).removeFromTop(sliderHeight).translated(0, yOffset));};

    // Apply layout to each band
    layoutBand(lowBandArea, lowThreshold, lowAttack, lowRelease, lowRatio);
    layoutBand(midBandArea, midThreshold, midAttack, midRelease, midRatio);
    layoutBand(highBandArea, highThreshold, highAttack, highRelease, highRatio);

    // Reset area for crossover section below
    area = getLocalBounds().reduced(10);
    area.removeFromTop(150); // Skip the band controls area (adjust height as needed)
    area.removeFromTop(20);  // Gap between sections

    // Crossover section
    auto crossoverArea = area.removeFromBottom(50); // 80px height for crossover section

    // Labels above the slider
	auto labelArea = crossoverArea.removeFromBottom(40); // 40px height for labels
    int labelWidth = labelArea.getWidth() / 2;
    lowCrossoverLabel.setBounds(labelArea.removeFromLeft(labelWidth));
    highCrossoverLabel.setBounds(labelArea.removeFromLeft(labelWidth));

    // Crossover slider below labels
    crossoverSlider.setBounds(crossoverArea.removeFromTop(40));
}