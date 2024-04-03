#pragma once

#include "State.hpp"

#if BAUD_RATE == 1
#define SERIAL_SPEED 230400
#elif BAUD_RATE == 2
#define SERIAL_SPEED 460800
#elif BAUD_RATE == 4
#define SERIAL_SPEED 921600
#elif BAUD_RATE == 5
#define SERIAL_SPEED 1000000
#elif BAUD_RATE == 6
#define SERIAL_SPEED 1500000
#elif BAUD_RATE == 7
#define SERIAL_SPEED 2000000
#elif BAUD_RATE == 8
#define SERIAL_SPEED 115200
#else
#define SERIAL_SPEED 500000
#define BAUD_RATE 3
#endif
