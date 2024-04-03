#pragma once

#include "Configuration.hpp"
#include <NeoPixelBus.h>

#if COLOR_MODE == 1
    #if COLOR_ORDER == 1
    #define NEO_FEATURE NeoGrbFeature
    #elif COLOR_ORDER == 2
    #define NEO_FEATURE NeoRgbFeature
    #elif COLOR_ORDER == 3
    #define NEO_FEATURE NeoBgrFeature
    #elif COLOR_ORDER == 4
    #define NEO_FEATURE NeoBrgFeature
    #elif COLOR_ORDER == 5
    #define NEO_FEATURE NeoRbgFeature
    #elif COLOR_ORDER == 6
    #define NEO_FEATURE NeoGbrFeature
    #else
    #define NEO_FEATURE NeoGrbFeature
    #define COLOR_ORDER 1
    #endif
#elif COLOR_MODE >= 2 && COLOR_MODE <= 4
    #if COLOR_ORDER == 1
    #define NEO_FEATURE NeoGrbwFeature
    #elif COLOR_ORDER == 2
    #define NEO_FEATURE NeoRgbwFeature
    #elif COLOR_ORDER == 3
    #define NEO_FEATURE NeoBgrwFeature
    #elif COLOR_ORDER == 4
    #define NEO_FEATURE NeoBrgwFeature
    #elif COLOR_ORDER == 5
    #define NEO_FEATURE NeoRbgwFeature
    #elif COLOR_ORDER == 6
    #define NEO_FEATURE NeoGbrwFeature
    #else
    #define NEO_FEATURE NeoGrbwFeature
    #define COLOR_ORDER 1
    #endif
#else
    #error "Unsupported COLOR_MODE"
#endif

enum class Effect {
    GlowWormWifi, GlowWorm, solid, fire, twinkle, bpm, rainbow, chase_rainbow, solid_rainbow, mixed_rainbow
};

class State {
public:
    NeoPixelBus<NEO_FEATURE, NEO_METHOD> strip = {LEDS, LED_PIN};

    uint8_t brightness = 255;
    uint8_t effectInUse = 1;
    Effect effect = Effect::GlowWorm;

    unsigned short frameCount = 0;
    unsigned long lastLedUpdate = 0;
    unsigned long lastConnectionCheck = 0;
    unsigned long lastSerialSend = 0;

    void checkConnection();
    void sendSerialInfo();

    /**
     * Apply white temp correction on RGB color and calculate W channel
     * colorMode can be: 1 = RGB, 2 = RGBW Accurate, 3 = RGBW Brighter, 4 = RGBW RGB only
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return RGBW color
     */
    RgbwColor calculateRgbwMode(uint8_t r, uint8_t g, uint8_t b);
    /**
     * Set pixel color
     * @param index LED num
     * @param r red channel
     * @param g green channel
     * @param b blu channel
     */
    void setPixelColor(uint16_t index, uint8_t rToOrder, uint8_t gToOrder, uint8_t bToOrder);
    void ledShow();
};

extern State state;
