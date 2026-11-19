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
    context(apvts),
    synth(context)
#endif
{
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
    synth.prepare(sampleRate);
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

    synth.process(buffer, midiMessages);
}

juce::AudioProcessorValueTreeState::ParameterLayout HelyumAudioProcessor::parameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    juce::StringArray waveChoices;
    for (int i = 0; i < WavetableLibrary::allCount; i++)
        waveChoices.add(WavetableLibrary::all[i].name);

    // Osc 1
    params.push_back(std::make_unique<juce::AudioParameterBool>(ParameterIDs::OSC1_ENABLE, "Osc 1 Active", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC1_VOLUME, "Osc 1 Volume", -60.0f, 6.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC1_PAN, "Osc 1 Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC1_WAVE_FRAME_ID, "Osc 1 wave frame", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(ParameterIDs::OSC1_WAVE_CHOOSE, "Osc 1 wavetable", waveChoices, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(ParameterIDs::OSC1_TRANSPOSE, "Osc 1 Transpose", -48, 48, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC1_TUNE, "Osc 1 Tune", -1, 1, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC1_PHASE_MULT, "Osc 1 Phase", 0.0f, 1.0f, 1.0f));

    // Osc 2
    params.push_back(std::make_unique<juce::AudioParameterBool>(ParameterIDs::OSC2_ENABLE, "Osc 2 Active", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC2_VOLUME, "Osc 2 Volume", -60.0f, 6.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC2_PAN, "Osc 2 Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC2_WAVE_FRAME_ID, "Osc 2 wave frame", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(ParameterIDs::OSC2_WAVE_CHOOSE, "Osc 2 wavetable", waveChoices, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(ParameterIDs::OSC2_TRANSPOSE, "Osc 2 Transpose", -48, 48, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC2_TUNE, "Osc 2 Tune", -1, 1, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ParameterIDs::OSC2_PHASE_MULT, "Osc 1 Phase", 0.0f, 1.0f, 1.0f));

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
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HelyumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr)
    {
        auto state = juce::ValueTree::fromXml(*xml);

        if (state.isValid())
            apvts.replaceState(state);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelyumAudioProcessor();
}
