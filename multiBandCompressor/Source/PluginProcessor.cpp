/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiBandCompressorAudioProcessor::MultiBandCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif

#endif
    ),
    parameters(*this, nullptr, "PARAMS", {
    //lowband
    std::make_unique<juce::AudioParameterFloat>("low_threshold", "Low Threshold", -60.0f, 0.0f,-24.0f), //id,label,min,max and starting
    std::make_unique<juce::AudioParameterFloat>("low_attack", "Low band attack", 1.0f, 200.0f, 20.0f),
    std::make_unique<juce::AudioParameterFloat>("low_release", "Low band release", 10.0f, 500.0f, 100.0f),
    std::make_unique<juce::AudioParameterFloat>("low_ratio", "Low band ratio", 1.0f, 20.0f, 2.0f),

    //midband
    std::make_unique<juce::AudioParameterFloat>("mid_threshold", "mid Threshold", -60.0f, 0.0f,-24.0f), //id,label,min,max and starting
    std::make_unique<juce::AudioParameterFloat>("mid_attack", "mid band attack", 1.0f, 200.0f, 20.0f),
    std::make_unique<juce::AudioParameterFloat>("mid_release", "mid band release", 10.0f, 500.0f, 100.0f),
    std::make_unique<juce::AudioParameterFloat>("mid_ratio", "mid band ratio", 1.0f, 20.0f, 2.0f),

    //highband
    std::make_unique<juce::AudioParameterFloat>("high_threshold", "High Threshold", -60.0f, 0.0f,-24.0f), //id,label,min,max and starting
    std::make_unique<juce::AudioParameterFloat>("high_attack", "high band attack", 1.0f, 200.0f, 20.0f),
    std::make_unique<juce::AudioParameterFloat>("high_release", "high band release", 10.0f, 500.0f, 100.0f),
    std::make_unique<juce::AudioParameterFloat>("high_ratio", "high band ratio", 1.0f, 20.0f, 2.0f),

    //crossover frequencies
    std::make_unique<juce::AudioParameterFloat>("low_mid_freq", "Low-Mid Crossover Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 300.0f),20.0f),  
	std::make_unique<juce::AudioParameterFloat>("mid_high_freq", "Mid-High Crossover Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 300.0f),20.0f),
        })
{
}

MultiBandCompressorAudioProcessor::~MultiBandCompressorAudioProcessor()
{
}

//==============================================================================
const juce::String MultiBandCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultiBandCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultiBandCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultiBandCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultiBandCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultiBandCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultiBandCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultiBandCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultiBandCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultiBandCompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void MultiBandCompressorAudioProcessor::updateCompressor()
{
    // Here you can update your filter parameters based on the cutoff value.
    // For example, if you have a filter object, you might do something like:
    // filter.setCutoffFrequency(*cutoffParam);
   // You should use this method to store your parameters in the memory block.
   // You could do that either as raw data, or use the XML or ValueTree classes
   // as intermediaries to make it easy to save and load complex data.


 //get the low-band compressor values
    float low_threshold = parameters.getRawParameterValue("low_threshold")->load();
    float low_attack = parameters.getRawParameterValue("low_attack")->load();
    float low_release = parameters.getRawParameterValue("low_release")->load();
    float low_ratio = parameters.getRawParameterValue("low_ratio")->load();

	//get the mid-band compressor values   
	float mid_threshold = parameters.getRawParameterValue("mid_threshold")->load();
	float mid_attack = parameters.getRawParameterValue("mid_attack")->load();
	float mid_release = parameters.getRawParameterValue("mid_release")->load();
	float mid_ratio = parameters.getRawParameterValue("mid_ratio")->load();

	//get the high-band compressor values
	float high_threshold = parameters.getRawParameterValue("high_threshold")->load();
	float high_attack = parameters.getRawParameterValue("high_attack")->load();
	float high_release = parameters.getRawParameterValue("high_release")->load();
	float high_ratio = parameters.getRawParameterValue("high_ratio")->load();

	//set the low-band compressor parameters
	lowBandCompressor.setThreshold(low_threshold);
	lowBandCompressor.setAttack(low_attack);
	lowBandCompressor.setRelease(low_release);
	lowBandCompressor.setRatio(low_ratio);

	//set the mid-band compressor parameters
	midBandCompressor.setThreshold(mid_threshold);
	midBandCompressor.setAttack(mid_attack);
	midBandCompressor.setRelease(mid_release);
	midBandCompressor.setRatio(mid_ratio);

	//set the high-band compressor parameters
	highBandCompressor.setThreshold(high_threshold);
	highBandCompressor.setAttack(high_attack);
	highBandCompressor.setRelease(high_release);
	highBandCompressor.setRatio(high_ratio);
}
void MultiBandCompressorAudioProcessor::updateCrossover()
{
    // Here you can update your filter parameters based on the cutoff value.
    // For example, if you have a filter object, you might do something like:
    // filter.setCutoffFrequency(*cutoffParam);
   // You should use this method to store your parameters in the memory block.
   // You could do that either as raw data, or use the XML or ValueTree classes
   // as intermediaries to make it easy to save and load complex data.


 //get the crossover values
    float lowmidFreq = parameters.getRawParameterValue("low_mid_freq")->load();
    float midhighFreq = parameters.getRawParameterValue("mid_high_freq")->load();

    //set the crossover frequencies
    lowMidCrossover.setCutoffFrequency(lowmidFreq);
    midHighCrossover.setCutoffFrequency(midhighFreq);

   
    //lowband
	lowMidCrossover.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
	lowMidCrossover.setCutoffFrequency(lowmidFreq);

    //midband
	lowHighCrossover.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
	lowHighCrossover.setCutoffFrequency(midhighFreq);//low part of high crossover
    highLowCrossover.setType(juce::dsp::LinkwitzRileyFilterType::highpass);//high part of low crossover
    highLowCrossover.setCutoffFrequency(lowmidFreq);

    //highband
	midHighCrossover.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    midHighCrossover.setCutoffFrequency(midhighFreq);
	

}

//==============================================================================
void MultiBandCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    lowMidCrossover.prepare(spec);
    lowHighCrossover.prepare(spec);
    highLowCrossover.prepare(spec);
    midHighCrossover.prepare(spec);
	lowBandCompressor.prepare(spec);
	midBandCompressor.prepare(spec);
	highBandCompressor.prepare(spec);
}

void MultiBandCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultiBandCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MultiBandCompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    updateCrossover();
    updateCompressor();

    // Create separate buffers for each band
    juce::AudioBuffer<float> lowBandBuffer, midBandBuffer, highBandBuffer;
    lowBandBuffer.makeCopyOf(buffer);
    midBandBuffer.makeCopyOf(buffer);
    highBandBuffer.makeCopyOf(buffer);

    // Create AudioBlocks for DSP processing
    juce::dsp::AudioBlock<float> lowBlock(lowBandBuffer);
    juce::dsp::AudioBlock<float> midBlock(midBandBuffer);
    juce::dsp::AudioBlock<float> highBlock(highBandBuffer);

    // Create processing contexts
    juce::dsp::ProcessContextReplacing<float> lowContext(lowBlock);
    juce::dsp::ProcessContextReplacing<float> midContext(midBlock);
    juce::dsp::ProcessContextReplacing<float> highContext(highBlock);

    // Apply your crossover filters using your existing logic:

    // LOW BAND: Low-pass at low-mid frequency
    lowMidCrossover.process(lowContext);

    // MID BAND: High-pass at low-mid frequency AND low-pass at mid-high frequency
    highLowCrossover.process(midContext);  // High-pass at low-mid freq
    lowHighCrossover.process(midContext);  // Low-pass at mid-high freq

    // HIGH BAND: High-pass at mid-high frequency
    midHighCrossover.process(highContext);

    // Apply compression to each band
    lowBandCompressor.process(lowContext);
    midBandCompressor.process(midContext);
    highBandCompressor.process(highContext);

    // Clear the output buffer first
    buffer.clear();

    // Mix the processed bands back together
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        buffer.addFrom(channel, 0, lowBandBuffer, channel, 0, buffer.getNumSamples());
        buffer.addFrom(channel, 0, midBandBuffer, channel, 0, buffer.getNumSamples());
        buffer.addFrom(channel, 0, highBandBuffer, channel, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool MultiBandCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiBandCompressorAudioProcessor::createEditor()
{
    return new MultiBandCompressorAudioProcessorEditor (*this);
}

//==============================================================================
void MultiBandCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultiBandCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiBandCompressorAudioProcessor();
}
