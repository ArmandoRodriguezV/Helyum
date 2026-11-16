#include "SynthSound.h"

bool SynthSound::appliesToChannel(int midiChannel)
{
    return true;
}

bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return true;
}
