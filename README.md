# glow_worm_luciferin_nano
Glow Worm Luciferin firmware specifically written for Arduino Nano ATMEGA386P

## Features
* Compatible with [Luciferin Firefly](https://github.com/sblantipodi/firefly_luciferin) v2.14.3 (firmware v5.13.2)
* Basic Bias Lighting and Ambient Light support with brightness
* Configurable though [configuration header file](/include/Configuration.hpp)
* Support for different temperatures for white light in RGBW LEDs

Tested with 60fps RGBW 108 LED strip and Arduino Nano ATMEGA386P.

## Missing features
* LED number/pin, color mode, color order are only configurable though [configuration header file](/include/Configuration.hpp) (Luciferin can't change these options on the fly)
* Only support thought USB connection
* Color temperature is unchangable
* Effects besides solid color won't work
* No DotStar LED strip support
