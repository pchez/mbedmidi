#include "mbed.h"

#define MIDI_BAUD_RATE                      31250
#define DEFAULT_ACTIVE_MIDI_CHANNEL         0x01
#define DEFAULT_MIDI_MIN_KEY_VAL            0
#define DEFAULT_MIDI_MAX_KEY_VAL            64
#define DEFAULT_MIDI_MIN_VELOCITY           0
#define DEFAULT_MIDI_MAX_VELOCITY           127

// Device specific defines
#define DEFAULT_DEVICE_NUM_KEYS             1

// MIDI Commands
#define MIDI_CMD_NOTE_ON                    0x90
#define MIDI_CMD_NOTE_OFF                   0x80

class MidiDevice
{
public:
    MidiDevice(uint8_t minKeyVal, uint8_t maxKeyVal) 
    : _uartToMidi(PD_5, PD_6, MIDI_BAUD_RATE), _debugDevice(USBTX, USBRX),
      _button(PF_13), _led(LED1), _lightSensor(A0)
    {
        _channelsActive = DEFAULT_ACTIVE_MIDI_CHANNEL;
        _minKeyVal = minKeyVal;
        _maxKeyVal = maxKeyVal;
        _numKeys = DEFAULT_DEVICE_NUM_KEYS;
        _lastNotePlayed = DEFAULT_MIDI_MIN_KEY_VAL;
        _keyStatus = 0;
    }
    void scanInput();
    uint32_t readLightSensor();

    void noteOn(uint8_t channel, uint8_t key, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t key, uint8_t velocity);

    
private:
    DigitalIn _button;
    AnalogIn _lightSensor;
    DigitalOut _led;
    RawSerial _uartToMidi;
    RawSerial _debugDevice;
    uint16_t _channelsActive;
    uint8_t _minKeyVal;
    uint8_t _maxKeyVal;
    uint8_t _numKeys;
    uint8_t _lastNotePlayed; // temporary, need to change to bitfield
    uint64_t _keyStatus;
};