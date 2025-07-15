/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterDesignAudioProcessorEditor::FilterDesignAudioProcessorEditor (FilterDesignAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    lowPassSlider.setSliderStyle(juce::Slider::LinearVertical);
    lowPassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    lowPassSlider.setRange(20.0f, 20000.0f, 1.0f);

	addAndMakeVisible(lowPassSlider);
    
    lowPassSlider.setLookAndFeel(&customLook);
    lowPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    audioProcessor.getParameters(), "low_cutoff", lowPassSlider);

    highPassSlider.setSliderStyle(juce::Slider::LinearVertical);
    highPassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    highPassSlider.setRange(20.0f, 20000.0f, 1.0f);

    addAndMakeVisible(highPassSlider);
    highPassSlider.setLookAndFeel(&customLook);
    highPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "high_cutoff", highPassSlider);

   
    setSize (400, 300);
}

FilterDesignAudioProcessorEditor::~FilterDesignAudioProcessorEditor()
{
}

//==============================================================================
void FilterDesignAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Filter high pass", getLocalBounds(), juce::Justification::centredTop, 1);
}

void FilterDesignAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //lowPassSlider.setBounds(getWidth() / 40- 75, getHeight() / 2 - 75, 150, 150);
    //highPassSlider.setBounds(getWidth() / 120 - 150, getHeight() / 2 - 75, 150, 150);
    int sliderWidth = 150;
    int sliderHeight = 150;
    int spacing = 50;

    int totalWidth = 2 * sliderWidth + spacing;
    int startX = (getWidth() - totalWidth) / 2;

    lowPassSlider.setBounds(startX, getHeight() / 2 - sliderHeight / 2, sliderWidth, sliderHeight);
    highPassSlider.setBounds(startX + sliderWidth + spacing, getHeight() / 2 - sliderHeight / 2, sliderWidth, sliderHeight);

}
