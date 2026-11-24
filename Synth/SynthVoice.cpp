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

void SynthVoice::prepare(int samplesPerBlock)
{
    voiceBuffer.setSize(2, samplesPerBlock);
    voiceBuffer.clear();
}

void SynthVoice::setCurrentPlaybackSampleRate(double newRate) {

    juce::SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);
    const auto wt1 = WavetableLibrary::find(0);
    const auto wt2 = WavetableLibrary::find(0);

    if (wt1 == nullptr || wt2 == nullptr)
        return;

    osc1Wavetable = Wavetable::loadWavetable(wt1->data, wt1->size);
    osc2Wavetable = Wavetable::loadWavetable(wt2->data, wt2->size);

    for (int ch = 0; ch < 2; ++ch)
    {
        osc1[ch].prepare(newRate);
        osc2[ch].prepare(newRate);
        osc1[ch].setWavetable(osc1Wavetable);
        osc2[ch].setWavetable(osc2Wavetable);
    }

    Wavetable::wavetableOsc1 = osc1Wavetable[0];
    Wavetable::wavetableOsc2 = osc2Wavetable[0];

    adsr.prepare(newRate);
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

    voiceBuffer.clear(0, numSamples);

    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = voiceBuffer.getWritePointer(ch);

        if (enableOsc1)
            osc1[ch].process(data, numSamples);

        if (enableOsc2)
            osc2[ch].process(data, numSamples);

        for (int i = 0; i < numSamples; ++i)
            data[i] *= velocity;
    }

    processAdsr(voiceBuffer, 0, numSamples);

    for (int ch = 0; ch < 2; ++ch)
    {
        outputBuffer.addFrom(ch, startSample, voiceBuffer, ch, 0, numSamples);
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

void SynthVoice::processAdsr(juce::AudioBuffer<float>& buffer, int startSample, int numSamples) {
    adsr.setAttack(params.get(ParameterIDs::ADSR_A));
    adsr.setDecay(params.get(ParameterIDs::ADSR_D));
    adsr.setSustain(params.get(ParameterIDs::ADSR_S));
    adsr.setRelease(params.get(ParameterIDs::ADSR_R));

    adsr.setAttackCurve(params.get(ParameterIDs::ADSR_A_CURVE));
    adsr.setDecayCurve(params.get(ParameterIDs::ADSR_D_CURVE));
    adsr.setReleaseCurve(params.get(ParameterIDs::ADSR_R_CURVE));

    adsr.process(buffer, startSample, numSamples);

    if (!adsr.active())
        clearCurrentNote();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);
    if (allowTailOff)
        adsr.end();
    else
        clearCurrentNote();
}
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    this->velocity = velocity;
    adsr.start();

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
