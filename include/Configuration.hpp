//  ------------------------------------------------------------
//  You should change fields below to match your configuration.
//  Since ATMEGA386P does not have dedicated storage memory
//  configuration is hardcoded in the code.
//  ------------------------------------------------------------

#pragma once
#include <stdint.h>

const uint16_t LEDS = 108; // Number of leds in your strip
const uint8_t LED_PIN = 13; // Pin where your leds are connected

// 1 - 230400
// 2 - 460800
// 3 - 500000
// 4 - 921600
// 5 - 1000000
// 6 - 1500000
// 7 - 2000000
// 8 - 115200
#define BAUD_RATE 3 // It is safer to leave it as it is

// 1 - RGB
// 2 - RGBW
// 3 - RGBW Mixed (Brighter)
// 4 - RGBW (RGB Only)
// 5 - Dotstar
#define COLOR_MODE 2

// 1 - GRB
// 2 - RGB
// 3 - BGR
// 4 - BRG
// 5 - RBG
// 6 - GBR
#define COLOR_ORDER 1

// See https://github.com/Makuna/NeoPixelBus/wiki/Neo-Methods
#define NEO_METHOD NeoWs2812xMethod

// If you're experiencing issues with your leds, you can try to disable reporting
// I helped me once, but later I didn't had same issue
// WARNING! Because of this option, Luciferin won't show arduino in devices list when connected
// Also will cause Luciferin to crash if you'll try to change settings
#define DISABLE_REPORTING_WHILE_UPDATING 0 // disables unnecessary reporting when leds are updating (but not frametime)
#define DISABLE_FRAMERATE_REPORTING 0 // disables completely framerate reporting

// If you have an RGBW strip, you can set the white temperature
// to adjust the white color
// https://andi-siess.de/rgb-to-color-temperature/
// Reference, currently set to 4500k white light (RGBWW)
const uint8_t kWhiteRedChannel = 255;
const uint8_t kWhiteGreenChannel = 219;
const uint8_t kWhiteBlueChannel = 186;

// If Arduino can't connect to Luciferin
// it will go to sleep mode
#define ENABLE_SLEEP_MODE 1

//  ------------------------------------------------------------
//  You can change fields below, but it is not recommended.
//  ------------------------------------------------------------

#define VERSION "5.13.2"
#define FIRMWARE_TYPE "NANO" 
#define BOARD "ATMEGA328P"

#define SERIAL_RX_BUFFER_SIZE 128 + LEDS * 3
#define SERIAL_TX_BUFFER_SIZE 196