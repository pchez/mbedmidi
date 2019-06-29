/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "MidiDevice.h"

MidiDevice ButtonMidi(DEFAULT_MIDI_MIN_KEY_VAL, DEFAULT_MIDI_MAX_KEY_VAL);

// main() runs in its own thread in the OS
int main()
{
    while (true)
    {
        ButtonMidi.scanInput();
        wait_ms(100);
    }
}
