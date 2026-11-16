#include "SynthVoice.h"

#include "../Resources/ParameterIDs.h"

SynthVoice::SynthVoice(ParameterContext &params)
    :params(params)
{
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::setCurrentPlaybackSampleRate(double newRate)
{
    juce::SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);
    for (int i = 0; i < 2; i++)
    {
        osc1[i].prepare(newRate);
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    totalChannels = outputBuffer.getNumChannels();

    const float volume = params.get(ParameterIDs::OSC1_VOLUME);
    const auto gain = juce::Decibels::decibelsToGain(volume, -60.0f);

    for (int ch = 0; ch < totalChannels; ++ch)
    {
        auto* data = outputBuffer.getWritePointer(ch);

        osc1[ch].setGain(gain);
        osc1[ch].setWaveFrame(params.get(ParameterIDs::OSC_WAVE_FRAME_ID));

        osc1[ch].process(data, numSamples);

        for (int i = startSample; i < startSample + numSamples; ++i)
            data[i] *= velocity;
    }
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    clearCurrentNote();
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    this->velocity = velocity;

    float freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    for (int ch = 0; ch < totalChannels; ++ch)
    {
        osc1[ch].setFrequency(freq);
        osc1[ch].reset();
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}
