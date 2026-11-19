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
        osc2[ch].prepare(newRate);
        // Load wt

        const auto wt1 = WavetableLibrary::find(0);
        osc1Wavetable = Wavetable::loadWavetable(wt1->data, wt1->size);
        osc1[ch].setWavetable(osc1Wavetable);

        const auto wt2 = WavetableLibrary::find(0);
        osc2Wavetable = Wavetable::loadWavetable(wt2->data, wt2->size);
        osc2[ch].setWavetable(osc2Wavetable);
    }

    Wavetable::wavetableOsc1 = osc1Wavetable[0];
    Wavetable::wavetableOsc2 = osc2Wavetable[0];
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    const bool enableOsc1 = params.get(ParameterIDs::OSC1_ENABLE);
    const bool enableOsc2 = params.get(ParameterIDs::OSC2_ENABLE);

    totalChannels = outputBuffer.getNumChannels();

    for (int ch = 0; ch < totalChannels; ++ch)
    {
        auto* data = outputBuffer.getWritePointer(ch);

        if (enableOsc1)
            osc1[ch].process(data + startSample, numSamples);

        if (enableOsc2)
            osc2[ch].process(data + startSample, numSamples);

        for (int i = 0; i < numSamples; ++i)
            data[startSample + i] *= velocity;
    }
}

void SynthVoice::alwaysProcess()
{
    osc1Processor();
    osc2Processor();
}

void SynthVoice::osc1Processor()
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

    // Transpose
    const int transpose = params.get(ParameterIDs::OSC1_TRANSPOSE);

    // Tune
    const float tune = params.get(ParameterIDs::OSC1_TUNE);

    if (wavetableChanged)
        lastWavetableIDOsc1 = id;

    for (int ch = 0; ch < totalChannels; ++ch)
    {
        osc1[ch].setWaveFrame(waveFrame);
        osc1[ch].setTranspose(transpose);
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

void SynthVoice::osc2Processor()
{
    // Gain
    const float volume = params.get(ParameterIDs::OSC2_VOLUME);
    const float gain = juce::Decibels::decibelsToGain(volume, -60.0f);

    // Pan
    const float pan = params.get(ParameterIDs::OSC2_PAN);
    const float angle = (pan + 1.0f) * Constants::PI * 0.25f;
    const float leftPan = std::cos(angle);
    const float rightPan = std::sin(angle);

    // Wave frame
    const float waveFrame = params.get(ParameterIDs::OSC2_WAVE_FRAME_ID);
    if (waveFrame != lastWaveFrameOsc2)
        Wavetable::wavetableOsc2 = osc2Wavetable[osc2[0].getFrame()];

    // Wavetable change
    const int id = params.get(ParameterIDs::OSC2_WAVE_CHOOSE);
    const bool wavetableChanged = id != lastWavetableIDOsc2;

    // Transpose
    const int transpose = params.get(ParameterIDs::OSC2_TRANSPOSE);

    // Tune
    const float tune = params.get(ParameterIDs::OSC2_TUNE);

    if (wavetableChanged)
        lastWavetableIDOsc2 = id;

    for (int ch = 0; ch < totalChannels; ++ch)
    {
        osc2[ch].setWaveFrame(waveFrame);
        osc2[ch].setTranspose(transpose);
        osc2[ch].setTune(tune);

        // Setters
        if (ch == 0)
            osc2[ch].setGain(gain * leftPan);
        else
            osc2[ch].setGain(gain * rightPan);


        if (wavetableChanged)
        {
            const auto wt = WavetableLibrary::find(id);
            osc2Wavetable = Wavetable::loadWavetable(wt->data, wt->size);
            osc2[ch].setWavetable(osc2Wavetable);
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

    const float osc1Phase = random.nextFloat() * params.get(ParameterIDs::OSC1_PHASE_MULT);
    const float osc2Phase = random.nextFloat() * params.get(ParameterIDs::OSC2_PHASE_MULT);

    const auto freq = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    for (int ch = 0; ch < totalChannels; ++ch)
    {
        osc1[ch].setFrequency(freq);
        osc2[ch].setFrequency(freq);

        osc1[ch].setPhase(osc1Phase);
        osc2[ch].setPhase(osc2Phase);
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}
