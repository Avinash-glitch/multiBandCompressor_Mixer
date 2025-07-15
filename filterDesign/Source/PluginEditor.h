/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomSliderLook.h"

//==============================================================================
/**
*/


class FilterDesignAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FilterDesignAudioProcessorEditor (FilterDesignAudioProcessor&);
    ~FilterDesignAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterDesignAudioProcessor& audioProcessor;
    juce::Slider highPassSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highPassAttachment;
    juce::Slider lowPassSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPassAttachment;
    CustomSliderLook customLook;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterDesignAudioProcessorEditor)
};
