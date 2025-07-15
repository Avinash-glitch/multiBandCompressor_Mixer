/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MultiBandCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor,
	                                             public juce::Slider::Listener
{
public:
    MultiBandCompressorAudioProcessorEditor (MultiBandCompressorAudioProcessor&);
    ~MultiBandCompressorAudioProcessorEditor() override;
	//==============================================================================
    juce::Slider crossoverSlider; // Slider for crossover frequency
    // Text labels for the crossover values
    juce::Label lowCrossoverLabel;
    juce::Label highCrossoverLabel;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultiBandCompressorAudioProcessor& audioProcessor;
	
    // LOW BAND
    juce::Slider lowThreshold, lowAttack, lowRelease, lowRatio;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowRatioAttachment;

    // MID BAND
    juce::Slider midThreshold, midAttack, midRelease, midRatio;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midRatioAttachment;

    // HIGH BAND
    juce::Slider highThreshold, highAttack, highRelease, highRatio;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highRatioAttachment;

	//juce::Slider crossoverSlider; // Slider for crossover frequency
 //   // Text labels for the crossover values
 //   juce::Label lowCrossoverLabel;
 //   juce::Label highCrossoverLabel;

    // Slider attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midHighAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiBandCompressorAudioProcessorEditor)
};
