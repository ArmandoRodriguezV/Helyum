#include "ElyumSynth.h"

ElyumSynth::ElyumSynth(ParameterContext& context)
{
    constexpr int numVoices = 16;

    for (int i = 0; i < numVoices; ++i)
    {
        auto voice = new SynthVoice(context);
        voices.push_back(voice);
        synth.addVoice(voice);
    }

    synth.addSound(new SynthSound());
}

ElyumSynth::~ElyumSynth()
{
}

void ElyumSynth::prepare(const double sP, const int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sP);
    for (auto voice : voices)
    {
        voice->setCurrentPlaybackSampleRate(sP);
    }
}

void ElyumSynth::process(juce::AudioBuffer<float> &buffer, const juce::MidiBuffer &midiMessages)
{
    for (auto voice : voices)
    {
        voice->alwaysProcess();
    }
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}
