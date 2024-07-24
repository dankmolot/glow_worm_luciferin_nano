#include "main.hpp"

#define CONFIG_NUM_PARAMS 20

// This value must meet the one in Firefly Luciferin
// We are transferring byte via Serial, the maximum decimal number that can be represented with 1 byte is 255.
// Use a multiplier to set a much bigger number using only 2 bytes.
const int SERIAL_CHUNK_SIZE = 250;

byte config[CONFIG_NUM_PARAMS];
unsigned short loopStart, processStart, processTime, numLedFromLuciferin;
uint8_t hi, lo, chk, loSecondPart, usbBrightness, gpio, baudRate, whiteTemp, fireflyEffect,
        fireflyColorMode, fireflyColorOrder, ldrEn, ldrTo, ldrInt, ldrMn, ldrAction, relaySerialPin, sbSerialPin, ldrSerialPin, gpioClock;

State state;

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.setTimeout(10);
    state.strip.Begin();
    state.strip.Show();
}

uint8_t prefix[] = {'D', 'P', 's', 'o', 'f', 't'};

inline bool isDataReady() {
    return Serial.available() >= 6 && Serial.read() == prefix[0] && Serial.read() == prefix[1] && Serial.read() == prefix[2] && Serial.read() == prefix[3] && Serial.read() == prefix[4] && Serial.read() == prefix[5];
}

void loop() {
    loopStart = millis();

    state.updateStats();

    if (Serial.available() == 0) {
        state.checkConnection();
    } else if (isDataReady()) {
        int configLen = Serial.readBytes((byte *) config, CONFIG_NUM_PARAMS);
        if (configLen == CONFIG_NUM_PARAMS) {
            int i = 0;
            hi = config[i++];
            lo = config[i++];
            loSecondPart = config[i++];
            usbBrightness = config[i++];
            gpio = config[i++];
            baudRate = config[i++];
            whiteTemp = config[i++];
            fireflyEffect = config[i++];
            ldrEn = config[i++];
            ldrTo = config[i++];
            ldrInt = config[i++];
            ldrMn = config[i++];
            ldrAction = config[i++];
            fireflyColorMode = config[i++];
            fireflyColorOrder = config[i++];
            relaySerialPin = config[i++];
            sbSerialPin = config[i++];
            ldrSerialPin = config[i++];
            gpioClock = config[i++];
            chk = config[i++];

            if (!(chk != (hi ^ lo ^ loSecondPart ^ usbBrightness ^ gpio ^ baudRate ^ whiteTemp ^ fireflyEffect
                                   ^ ldrEn ^ ldrTo ^ ldrInt ^ ldrMn ^ ldrAction ^ fireflyColorMode ^ fireflyColorOrder
                                   ^ relaySerialPin ^ sbSerialPin ^ ldrSerialPin ^ gpioClock ^ 0x55))) {

                processStart = millis();
                numLedFromLuciferin = lo + (loSecondPart * SERIAL_CHUNK_SIZE) + 1;

                if (usbBrightness != state.brightness) {
                    state.brightness = usbBrightness;
                }

                if (fireflyEffect != 0 && state.effectInUse != fireflyEffect) {
                    state.effectInUse = fireflyEffect;
                    switch (fireflyEffect) {
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                            state.effect = Effect::GlowWorm;
                            break;
                        case 6:
                            state.effect = Effect::solid;
                            break;
                        case 7:
                            state.effect = Effect::fire;
                            break;
                        case 8:
                            state.effect = Effect::twinkle;
                            break;
                        case 9:
                            state.effect = Effect::bpm;
                            break;
                        case 10:
                            state.effect = Effect::rainbow;
                            break;
                        case 11:
                            state.effect = Effect::mixed_rainbow;
                            break;
                        case 12:
                            state.effect = Effect::chase_rainbow;
                            break;
                        case 13:
                            state.effect = Effect::solid_rainbow;
                            break;
                    }
                }

                if (numLedFromLuciferin == LEDS) {
                    byte step = state.strip.PixelSize();
                    byte* start = state.strip.Pixels();
                    byte* buffer = start;
                    byte* stop = buffer + step * numLedFromLuciferin;
                    while (buffer != stop) {
                        Serial.readBytes(buffer, 3);
                        buffer += step;
                    }

                    noInterrupts();

                    state.strip.Dirty();

                    int j = 0;
                    buffer = start;
                    while (buffer != stop) {
                        state.setPixelColor(j, buffer[0], buffer[1], buffer[2]);
                        buffer += step;
                        j++;
                    }

                    State::clearSerial(false);

                    state.ledShow();

                    interrupts();
                }

                processTime = millis() - processStart;
                state.totalProcessTime += processTime;
                state.lastProcessTime = processTime;
                state.highestProcessTime = max(state.highestProcessTime, processTime);
            }
        }
    }

    state.totalLoopTime += millis() - loopStart;
}
