#include "main.hpp"

#define CONFIG_NUM_PARAMS 20
#define CONFIG_PREFIX_LENGTH 6

// This value must meet the one in Firefly Luciferin
// We are transferring byte via Serial, the maximum decimal number that can be represented with 1 byte is 255.
// Use a multiplier to set a much bigger number using only 2 bytes.
const int SERIAL_CHUNK_SIZE = 250;

byte pre[CONFIG_PREFIX_LENGTH];
byte config[CONFIG_NUM_PARAMS];
uint8_t prefix[] = {'D', 'P', 's', 'o', 'f', 't'};
uint8_t hi, lo, chk, loSecondPart, usbBrightness, gpio, baudRate, whiteTemp, fireflyEffect,
        fireflyColorMode, fireflyColorOrder, ldrEn, ldrTo, ldrInt, ldrMn, ldrAction, relaySerialPin, sbSerialPin, ldrSerialPin, gpioClock;

State state;

void setup() {
    Serial.begin(SERIAL_SPEED);
    state.strip.Begin();
    state.strip.Show();
}

bool breakLoop = false;
void loop() {
    state.checkConnection();

    int i = 0;
    int prefixLength = Serial.readBytes((byte *) pre, CONFIG_PREFIX_LENGTH);
    bool prefixOk = false;
    if (prefixLength == CONFIG_PREFIX_LENGTH) {
        if (pre[0] == prefix[0] && pre[1] == prefix[1] && pre[2] == prefix[2] && pre[3] == prefix[3] &&
            pre[4] == prefix[4] && pre[5] == prefix[5]) {
            prefixOk = true;
        }
    }
    if (prefixOk) {
        int configLen = Serial.readBytes((byte *) config, CONFIG_NUM_PARAMS);
        if (configLen == CONFIG_NUM_PARAMS) {
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

            if (!(!breakLoop && (chk != (hi ^ lo ^ loSecondPart ^ usbBrightness ^ gpio ^ baudRate ^ whiteTemp ^ fireflyEffect
                                   ^ ldrEn ^ ldrTo ^ ldrInt ^ ldrMn ^ ldrAction ^ fireflyColorMode ^ fireflyColorOrder
                                   ^ relaySerialPin ^ sbSerialPin ^ ldrSerialPin ^ gpioClock ^ 0x55)))) {
                
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

                uint16_t numLedFromLuciferin = lo + (loSecondPart * SERIAL_CHUNK_SIZE) + 1;
                uint16_t j = 0;
                while (j < numLedFromLuciferin) {
                    byte r, g, b;
                    while (!Serial.available());
                    r = Serial.read();
                    while (!Serial.available());
                    g = Serial.read();
                    while (!Serial.available());
                    b = Serial.read();
                    state.setPixelColor(j++, r, g, b);
                }
                state.lastLedUpdate = millis();
#if !DISABLE_FRAMERATE_REPORTING
                state.frameCount++;
#endif

                if (state.effect != Effect::mixed_rainbow) {
                    state.ledShow();
                }
            }
        }
    }

    while (!breakLoop && Serial.available()) {
        Serial.read();
    }
}
