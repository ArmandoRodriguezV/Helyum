/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Resources/ParameterIDs.h"

//==============================================================================
HelyumAudioProcessor::HelyumAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "PARAMS", parameterLayout()),
    context(apvts)
#endif
{
    for (int i = 0; i < 8; ++i)
        synth.addVoice(new SynthVoice(context));

    synth.addSound(new SynthSound());
}

HelyumAudioProcessor::~HelyumAudioProcessor()
{
}

//==============================================================================
const juce::String HelyumAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HelyumAudioProcessor::acceptsMidi() const
{
   return true;
}

bool HelyumAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HelyumAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HelyumAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HelyumAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HelyumAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HelyumAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HelyumAudioProcessor::getProgramName (int index)
{
    return {};
}

void HelyumAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HelyumAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void HelyumAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HelyumAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HelyumAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

juce::AudioProcessorValueTreeState::ParameterLayout HelyumAudioProcessor::parameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC1_VOLUME, "Osc 1 Volume", -60.0f, 6.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC_WAVE_FRAME_ID, "Osc 1 wave frame", 0.0f, 1.0f, 0.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
bool HelyumAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelyumAudioProcessor::createEditor()
{
    return new HelyumAudioProcessorEditor (*this);
}

//==============================================================================
void HelyumAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HelyumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelyumAudioProcessor();
}
