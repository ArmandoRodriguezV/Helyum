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
    for (int ch = 0; ch < 2; ch++)
    {
        osc1[ch].prepare(newRate);
        // Load wt
        const auto wt = WavetableLibrary::find(0);
        osc1Wavetable = Wavetable::loadWavetable(wt->data, wt->size);
        osc1[ch].setWavetable(osc1Wavetable);
    }

    Wavetable::wavetableOsc1 = osc1Wavetable[0];
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    totalChannels = outputBuffer.getNumChannels();

    for (int ch = 0; ch < totalChannels; ++ch)
    {
        auto* data = outputBuffer.getWritePointer(ch);

        // Process
        osc1[ch].process(data + startSample, numSamples);

        // Update samples
        for (int i = 0; i < numSamples; ++i)
            data[startSample + i] *= velocity;
    }
}

void SynthVoice::alwaysProcess()
{
    // Gain
    const float volume = params.get(ParameterIDs::OSC1_VOLUME);
    const float gain = juce::Decibels::decibelsToGain(volume, -60.0f);

    // Pan
    const float pan = params.get(ParameterIDs::OSC1_PAN);
    const float angle = (pan + 1.0f) * Constants::PI * 0.25f;
    const float leftPan = std::cos(angle);
    const float rightPan = std::sin(angle);

    // Wave frame
    const float waveFrame = params.get(ParameterIDs::OSC1_WAVE_FRAME_ID);
    if (waveFrame != lastWaveFrameOsc1)
        Wavetable::wavetableOsc1 = osc1Wavetable[osc1[0].getFrame()];

    // Wavetable change
    const int id = params.get(ParameterIDs::OSC1_WAVE_CHOOSE);
    const bool wavetableChanged = id != lastWavetableIDOsc1;

    // Tune
    const int tune = params.get(ParameterIDs::OSC1_TUNE);

    if (wavetableChanged)
        lastWavetableIDOsc1 = id;

    for (int ch = 0; ch < totalChannels; ++ch)
    {
        osc1[ch].setWaveFrame(waveFrame);
        osc1[ch].setTune(tune);

        // Setters
        if (ch == 0)
            osc1[ch].setGain(gain * leftPan);
        else
            osc1[ch].setGain(gain * rightPan);


        if (wavetableChanged)
        {
            const auto wt = WavetableLibrary::find(id);
            osc1Wavetable = Wavetable::loadWavetable(wt->data, wt->size);
            osc1[ch].setWavetable(osc1Wavetable);
        }
    }
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    clearCurrentNote();
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    this->velocity = velocity;

    const auto freq = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    for (int ch = 0; ch < totalChannels; ++ch)
    {
        osc1[ch].setFrequency(freq);
        osc1[ch].reset();
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}
