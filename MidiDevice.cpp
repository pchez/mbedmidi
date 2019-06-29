#include "mbed.h"
#include "MidiDevice.h"

#define MIDI_KEY_WITHIN_RANGE(key, min, max)   ((key >= min) && (key <= max))
#define MIDI_CHANNEL_VALID(channel, channels_active) ((channel & channels_active) > 0)
#define isNewKeyPress(current_status) (current_status != kMidiInputStatusHold && current_status != kMidiInputStatusPress)

typedef enum
{
    kMidiInputStatusNone,
    kMidiInputStatusNewPress,
    kMidiInputStatusHold,
    kMidiInputStatusLiftoff,
    kMidiInputStatusMaxNum,
} MidiInputStatus;

//////////// Helpers ////////////
uint32_t MidiDevice::readLightSensor(void)
{
    return _lightSensor.read() * 100;
}

/////////// Member Functions /////////////
void MidiDevice::scanInput()
{
    uint8_t newKeyStatus = kMidiInputStatusNone;
    
    uint32_t note = 0;
    
    // Check button presses and update status accordingly
    if (!_button)
    {
        _led = 0;

        for (int i=0; i < DEFAULT_MIDI_MAX_KEY_VAL; i++)
        {
            if ( _keyStatus & (1 << i) )
            {
                noteOff(_channelsActive, i, DEFAULT_MIDI_MAX_VELOCITY);
                _keyStatus &= ~(1 << i);
            }
        }
    }
    else
    {
        _led = 1;
        note = readLightSensor();
        if ( !(_keyStatus & (1 << note)) )
        {
            noteOn(_channelsActive, note, DEFAULT_MIDI_MAX_VELOCITY);
            _keyStatus |= (1 << note);
        }
    }
}

void MidiDevice::noteOn(uint8_t channel, uint8_t key, uint8_t velocity)
{
    if ( MIDI_KEY_WITHIN_RANGE(key, _minKeyVal, _maxKeyVal) && MIDI_CHANNEL_VALID(channel, _channelsActive) )
    {
        _uartToMidi.putc(MIDI_CMD_NOTE_ON | channel);
        _uartToMidi.putc(key);
        _uartToMidi.putc(velocity);
    }
}

void MidiDevice::noteOff(uint8_t channel, uint8_t key, uint8_t velocity)
{
    // For now, transmit 0 velocity message for off
    if ( MIDI_KEY_WITHIN_RANGE(key, _minKeyVal, _maxKeyVal) && MIDI_CHANNEL_VALID(channel, _channelsActive) )
    {
        _uartToMidi.putc(MIDI_CMD_NOTE_OFF | channel);
        _uartToMidi.putc(key);
        _uartToMidi.putc(DEFAULT_MIDI_MIN_VELOCITY);
    }
}