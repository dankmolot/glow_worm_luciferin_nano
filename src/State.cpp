#include "State.hpp"
#include <Arduino.h>
#include "scale8.h"

#if ENABLE_SLEEP_MODE
#include <LowPower.h>
#endif

void State::checkConnection() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastConnectionCheck > 15000) {
        lastConnectionCheck = currentMillis;
        if (currentMillis > lastLedUpdate + 10000) {
            strip.ClearTo(0);
            ledShow();
        }
#if ENABLE_SLEEP_MODE
        if (currentMillis > lastLedUpdate + 30000) {
            // Go to sleep mode
            LowPower.powerStandby(SLEEP_8S, ADC_OFF, BOD_OFF);
        }
#endif
    }

    sendSerialInfo();
}

template<typename T>
void serialPrint(const char* message, T value) {
    Serial.print(message);
    Serial.println(value);
}

void State::sendSerialInfo() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastSerialSend > 10000) {
        lastSerialSend = currentMillis;

#if !DISABLE_FRAMERATE_REPORTING
        float framerate = frameCount > 0 ? static_cast<float>(frameCount) / 10 : 0;
        frameCount = 0;
        serialPrint("framerate:", framerate);
#endif

#if DISABLE_REPORTING_WHILE_UPDATING
        // Prevent sending too much information
        // Since it can corrupt the data we are receiving
        if (currentMillis - lastLedUpdate > 1000) {
#endif
            serialPrint("firmware:", FIRMWARE_TYPE);
            serialPrint("ver:", VERSION);
            serialPrint("board:", BOARD);
            serialPrint("lednum:", LEDS);
            serialPrint("gpio:", LED_PIN);
            serialPrint("baudrate:", BAUD_RATE);
            serialPrint("effect:", effectInUse);
            serialPrint("colorMode:", COLOR_MODE);
            serialPrint("colorOrder:", COLOR_ORDER);
#if DISABLE_REPORTING_WHILE_UPDATING
        }
#endif
    }
}

RgbwColor State::calculateRgbwMode(uint8_t r, uint8_t g, uint8_t b) {
#if COLOR_MODE == 2 || COLOR_MODE == 3
    uint8_t whiteValueForRed = scale8(r, kWhiteRedChannel);
    uint8_t whiteValueForGreen = scale8(g, kWhiteGreenChannel);
    uint8_t whiteValueForBlue = scale8(b, kWhiteBlueChannel);

    uint8_t minWhiteValue = min(whiteValueForRed,
                                 min(whiteValueForGreen,
                                     whiteValueForBlue));
#if COLOR_MODE == 2
    r = r - scale8(minWhiteValue, kWhiteRedChannel);
    g = g - scale8(minWhiteValue, kWhiteGreenChannel);
    b = b - scale8(minWhiteValue, kWhiteBlueChannel);
#endif

    return RgbwColor(r, g, b, minWhiteValue);
#else
    // Disable white color
    return RgbwColor(r, g, b, 0);
#endif
}


void State::setPixelColor(uint16_t index, uint8_t rToOrder, uint8_t gToOrder, uint8_t bToOrder) {
    // Apply brightness
    if (brightness != 255) {
        nscale8x3(rToOrder, gToOrder, bToOrder, brightness);
    }

#if COLOR_MODE == 1 || COLOR_MODE == 5
    strip.SetPixelColor( index, RgbColor(rToOrder, gToOrder, bToOrder) );
#elif COLOR_MODE >= 2 && COLOR_MODE <= 4
    strip.SetPixelColor( index, calculateRgbwMode(rToOrder, gToOrder, bToOrder) );
#endif
}

void State::ledShow() {
    strip.Show(false);
}
