/*******************************************
# Author        : Retuze 
# Date          : 2024-01-18 10:07:29
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-18 10:13:14
# Description   : 
*********************************************/
#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include "digital_io.h"
#include "wiring_time.h"
#include "HardwareSerial.h"
#include "HardwareTimer.h"
#include "HardwareSPI.h"

typedef uint8_t byte;

#define MOSI            PA7
#define MISO            PA6
#define SCK             PA5
#define SS              PA2
#define SDA             
#define SCL
#define PIN_SERIAL_RX   PA9
#define PIN_SERIAL_TX   PA10

#endif // _ARDUINO_H_