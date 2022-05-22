#define SKETCH "windStation"
/*
     From: DS2423WindSpeedCalibration
     Inspired by Joe Bechter

     Wind Speed:
     This sketch uses the DS2423 library and the Arduino
     OneWire library to read a counter value from a Dallas Semiconductor DS2423.
     This counter value is mapped to actual windspeed.

     Wind Direction

     Temperature
     There is a DS18S20 on the AAG Wind Station PCB.






*/

#include <Arduino.h>
#include <OneWire.h>
#include "DS2423.h"
#include <Ticker.h>

/*
  //For the datalogger
  #include <SPI.h>
  #include <SD.h>

  //Avoid these GPIOs in case we want to use the SD card shield again.
  //  Micro SD card pins:
  //  D5 = CLK
  //  D6 = MISO
  //  D7 = MOSI
  //  D8 = CS

  const int CHIPSELECT = D8;
*/

const uint8_t ONE_WIRE_PIN = D4;        // 1-Wire network

Ticker tickr1;


// Address of the DS2423 counter
uint8_t DS2423_address[] = { 0x1D, 0xF6, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x56 };

OneWire oneWire(ONE_WIRE_PIN);
DS2423 ds2423(&oneWire, DS2423_address);


int windSpeed = 0;          //The mapped wind speed
bool windSpeedFlag = true;  //Flag true with new reading
char buffer[20];




//======================= setup() ===============================
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);
  ds2423.begin();

  // Read the counter every 10.0s
  tickr1.attach(10.0, readWindspeed);

}





//======================= loop() ===============================
void loop() {
  if (windSpeedFlag) {
    windSpeedFlag = false;
    Serial.print("windSpeed= ");
    Serial.println(windSpeed);
  }
}



//======================= readWindspeed() ===============================
void readWindspeed() {
  // Take a reading of the DS2423 counter then
  // subtract the prior to get the speedRaw
  // speedRaw is the number of counts since the prior reading.
  // speedRaw is mapped to windSpeed
  //
  uint32_t countRaw = 0;      //The raw count from the DS2423
  static uint32_t countPrior = 0;    //The raw count from the last read
  int speedRaw = 0;           //The difference

  ds2423.update();
  if (ds2423.isError()) {
    Serial.println("Error reading counter");
  } else {
    countRaw = ds2423.getCount();
    speedRaw = countRaw - countPrior;
    countPrior = countRaw;
    //Serial.print("speedRaw= ");
    //Serial.println(speedRaw);
    windSpeed = speedRaw;
    map(windSpeed, 0, 88, 0, 100);
    windSpeedFlag = true;
  }
}
