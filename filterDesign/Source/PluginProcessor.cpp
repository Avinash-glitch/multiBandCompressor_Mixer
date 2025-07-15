/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomSliderLook.h"

//==============================================================================
FilterDesignAudioProcessor::FilterDesignAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
#endif
      ),

    parameters(*this, nullptr, "PARAMS", {
    std::make_unique<juce::AudioParameterFloat>(
        "high_cutoff",                 // ID
        "High Cutoff Frequency",       // Label
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.5f),
        500.0f                         // Default
    ),
    std::make_unique<juce::AudioParameterFloat>(
        "low_cutoff",                 // ID
        "Low Cutoff Frequency",       // Label
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.5f),
        500.0f                         // Default
    )
    // ,
    //std::make_unique<juce::AudioParameterChoice>(
    //    "filter_type",                // ID
    //    "Filter Type",                // Label
    //    juce::StringArray{ "LowPass", "HighPass" },
    //    0                              // Default index (LowPass)
    //)
        })

{
}

FilterDesignAudioProcessor::~FilterDesignAudioProcessor()
{
}

//==============================================================================
const juce::String FilterDesignAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterDesignAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterDesignAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterDesignAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterDesignAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterDesignAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterDesignAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterDesignAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterDesignAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterDesignAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}
//juce::dsp::IIR::Coefficients<float>::Ptr FilterDesignAudioProcessor::selectFilterType( const juce::String& filterType,float sampleRate, float cutoff)
//{
//	// This function can be used to select the filter type based on a parameter value.
//	// For example, you could have a parameter that selects between different filter types.
//	if (filterType == "HighPass")
//	{
//		// Set the filter type to high-pass
//        return juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, cutoff);
//	}
//	else if (filterType == "LowPass")
//	{
//		// Set the filter type to low-pass
//		return juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff);
//	}
//    else
//    {
//        return nullptr; // fallback
//    }
//}

void FilterDesignAudioProcessor::updateFilter()
{
    // Here you can update your filter parameters based on the cutoff value.
    // For example, if you have a filter object, you might do something like:
    // filter.setCutoffFrequency(*cutoffParam);
   // You should use this method to store your parameters in the memory block.
   // You could do that either as raw data, or use the XML or ValueTree classes
   // as intermediaries to make it easy to save and load complex data.

   

     auto highCutoff = parameters.getRawParameterValue("high_cutoff")->load();
     auto lowCutoff = parameters.getRawParameterValue("low_cutoff")->load();

     *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), highCutoff);
     *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), lowCutoff);
	//auto filterCoeffs = selectFilterType(filterType,getSampleRate(), cutoff);
    //*myFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), cutoff);
}
//==============================================================================
void FilterDesignAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    highPassFilter.prepare(spec);
    lowPassFilter.prepare(spec);
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 500.0f);
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 500.0f);
}

void FilterDesignAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterDesignAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FilterDesignAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    updateFilter();

    // Wrap the buffer into an AudioBlock for JUCE DSP processing
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // Process the audio block with the high-pass filter
    highPassFilter.process(context);
	lowPassFilter.process(context);
}

//==============================================================================
bool FilterDesignAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterDesignAudioProcessor::createEditor()
{
    return new FilterDesignAudioProcessorEditor (*this);
}

//==============================================================================
void FilterDesignAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilterDesignAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterDesignAudioProcessor();
}
