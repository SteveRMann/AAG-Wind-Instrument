#define SKETCH "DS2423WindSpeedCalibration"
/*
     DS2423WindSpeedCalibration

     This sketch uses the DS2423 library and the Arduino
     OneWire library to read a counter value from a Dallas Semiconductor DS2423.

     Calibration adds the Datalogger to record the raw speed vsalues
     Need to add GPS speed andheading

     USAGE:
     Run Termite on the tablet and plug the USB into the datalogger Wemos.
     Have a companion drive at 5, 10, 15 and 20 MPH and record the raw values for each speed.
     To be accurate and account for wind, drive each speed in a round trip.

     Inspired by Joe Bechter

*/

#include <Arduino.h>
#include <OneWire.h>
#include "DS2423.h"
#include <Ticker.h>

//For the datalogger
#include <SPI.h>
#include <SD.h>

/*
    Micro SD card pins:
    D5 = CLK
    D6 = MISO
    D7 = MOSI
    D8 = CS
*/
const int CHIPSELECT = D8;
const uint8_t ONE_WIRE_PIN = D4;        // 1-Wire network

Ticker tickr1;


// Address of the DS2423 counter
uint8_t DS2423_address[] = { 0x1D, 0xF6, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x56 };

OneWire oneWire(ONE_WIRE_PIN);
DS2423 ds2423(&oneWire, DS2423_address);

uint32_t countRaw = 0;      //The raw count from the DS2423
uint32_t countPrior = 0;    //The raw count from the last read
int speedRaw = 0;           //The difference
char buffer[20];




//======================= setup() ===============================
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);
  ds2423.begin();

  Serial.print("Initializing SD card...");

  // See if the card is present and can be initialized:
  if (!SD.begin(CHIPSELECT)) {
    Serial.println("SD Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  SD.remove("datalog.txt"); // delete the file if existed



  // Read the counter every 10.0s
  tickr1.attach(10.0, readWindspeed);
  
}





//======================= loop() ===============================
void loop() {
}




//======================= writeSD() ===============================
void writeSD() {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(speedRaw);
    dataFile.close();
  } else {
    Serial.println("error opening datalog.txt");
  }
}



//======================= readWindspeed() ===============================
void readWindspeed() {
  // Take a reading of the DS2423 counter then
  // subtract the prior to get the speedRaw
  // speedRaw is the number of counts since the prior reading.
  //
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
    writeSD();

//    map(value, fromLow, fromHigh, toLow, toHigh);
  }
}
