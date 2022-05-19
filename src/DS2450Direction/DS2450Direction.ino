/*
 *   DS2450Direction
 *   Original code: https://github.com/jbechter/arduino-onewire-DS2450
 *   
 *   This example demonstrates the use of the DS2450 library and the Arduino
 *   OneWire library to read wind vane direction from a Dallas Semiconductor DS2450
 *   quad a/d in an AAG wind vane.
 *
 *   by Joe Bechter
 *   (C) 2012, bechter.com
 *
 */

#include <Arduino.h>
#include <OneWire.h>
#include "DS2450.h"

// define the Arduino digital I/O pin to be used for the 1-Wire network here
const uint8_t ONE_WIRE_PIN = D4;

// define the 1-Wire address of the DS2450 quad a/d here (lsb first)
uint8_t DS2450_address[] = { 0x20, 0x27, 0xBF, 0x00, 0x00, 0x00, 0x00, 0xFC };

// Define North offset (0 to 15) here.
// To determine the offset, point the vane North.
// If the readout says WSW, then count how many steps to get to N
// So from WSW, to W, WNW all the way to N is 5 steps.
const int NORTH_OFFSET = 5;

OneWire ow(ONE_WIRE_PIN);
DS2450 ds2450(&ow, DS2450_address);

//Truth table for 16 discrete directions
//If the 4 a/d channels agree, then the vane is pointing in a discrete direction.
const float directionTable[16][4] = {
        { 4.5, 4.5, 2.5, 4.5 }, // N
        { 4.5, 2.5, 2.5, 4.5 }, // NNE
        { 4.5, 2.5, 4.5, 4.5 }, // NE
        { 2.5, 2.5, 4.5, 4.5 }, // ENE
        { 2.5, 4.5, 4.5, 4.5 }, // E
        { 2.5, 4.5, 4.5, 0.0 }, // ESE
        { 4.5, 4.5, 4.5, 0.0 }, // SE
        { 4.5, 4.5, 0.0, 0.0 }, // SSE
        { 4.5, 4.5, 0.0, 4.5 }, // S
        { 4.5, 0.0, 0.0, 4.5 }, // SSW
        { 4.5, 0.0, 4.5, 4.5 }, // SW
        { 0.0, 0.0, 4.5, 4.5 }, // WSW
        { 0.0, 4.5, 4.5, 4.5 }, // W
        { 0.0, 4.5, 4.5, 2.5 }, // WNW
        { 4.5, 4.5, 4.5, 2.5 }, // NW
        { 4.5, 4.5, 2.5, 2.5 }  // NNW
    };

const char directions[17][4] = {
        "N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
        "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW",
        "Err"
    };


void setup() {
    Serial.begin(115200);
    ds2450.begin();
}

void loop() {
    ds2450.update();
    if (ds2450.isError()) {
        Serial.println("Error reading from DS2450 device");
    } else {
        int direction = 16;
        float ch_a = ds2450.getVoltage(0);
        float ch_b = ds2450.getVoltage(1);
        float ch_c = ds2450.getVoltage(2);
        float ch_d = ds2450.getVoltage(3);
        for (int i = 0; i < 16; i++) {
            if (((ch_a < directionTable[i][0] + 1.0) && (ch_a >= directionTable[i][0] - 1.0)) &&
                    ((ch_b < directionTable[i][1] + 1.0) && (ch_b >= directionTable[i][1] - 1.0)) &&
                    ((ch_c < directionTable[i][2] + 1.0) && (ch_c >= directionTable[i][2] - 1.0)) &&
                    ((ch_d < directionTable[i][3] + 1.0) && (ch_d >= directionTable[i][3] - 1.0))) {
                direction = (i + NORTH_OFFSET) % 16;
                break;
            }
        }
        Serial.println(directions[direction]);
    }
    delay(500);
}
