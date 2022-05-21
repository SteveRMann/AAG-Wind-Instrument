#define SKETCH "DS2423WindSpeed"
/*
     DS2423WindSpeed

     This sketch uses the DS2423 library and the Arduino
     OneWire library to read a counter value from a Dallas Semiconductor DS2423.

     Inspired by Joe Bechter

*/

#include <Arduino.h>
#include <OneWire.h>
#include "DS2423.h"
#include <Ticker.h>

Ticker tickr;

const uint8_t ONE_WIRE_PIN = D4;        // 1-Wire network

// Address of the DS2423 counter
uint8_t DS2423_address[] = { 0x1D, 0xF6, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x56 };

OneWire oneWire(ONE_WIRE_PIN);
DS2423 ds2423(&oneWire, DS2423_address);

uint32_t countRaw = 0;      //The raw count from the DS2423
uint32_t countPrior = 0;    //The raw count from the last read
int speedRaw = 0;           //The difference

void readRaw() {
  // Take a reading of the counter then
  // subtract the prior to get the speedRaw
  ds2423.update();
  if (ds2423.isError()) {
    Serial.println("Error reading counter");
  } else {
    Serial.print("rawSpeed = ");
    //Serial.println(ds2423.getCount());
    countRaw = ds2423.getCount();
    speedRaw = countRaw - countPrior;
    countPrior = countRaw;
    Serial.println(speedRaw);
  }

}

//======================= setup() ===============================
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);
  ds2423.begin();

  // Read the counter every 10.0s
  tickr.attach(10.0, readRaw);
}


//======================= loop() ===============================
void loop() {

  delay(100);
}
