#define SKETCH "windStation"
/*
     Wind Speed:
     Read a counter value from a DS2423.
     This counter value is mapped to actual windspeed.

     Wind Direction
     This function reads the wind vane direction from a
     DS2450 quad a/d in an AAG wind vane.

     Temperature
     There is a DS18S20 on the AAG Wind Station PCB.

*/

#include <Arduino.h>
#include <OneWire.h>
#include <TickTwo.h>


//----- Prototypes (Must precede TickTwo timers -----
// Must preceed ticker calls.
void readWindDirection();
void readWindSpeed();
void wsTemperature(void);

TickTwo timer1(readWindSpeed, 10000);       //Get windspeed every ten seconds
TickTwo timer2(readWindDirection, 5000);    //Get wind direction every 5 seconds
TickTwo timer3(wsTemperature, 5000);        //Call myTemperature() every 5,000 ms.



//const uint8_t ONE_WIRE_BUS = D4;        // 1-Wire network
#define ONE_WIRE_BUS D4
OneWire oneWire(ONE_WIRE_BUS);



//--------------------------- Wind Speed ------------------------------------
#include "DS2423.h"

// Address of the DS2423 counter
uint8_t DS2423_address[] = { 0x1D, 0xF6, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x56 };
DS2423 ds2423(&oneWire, DS2423_address);

//Wind speed globals
uint32_t windSpeed = 0;           //The mapped wind speed
bool windSpeedFlag = false;       //Flag true with new reading



//--------------------------- Wind Direction ------------------------------------
#include "DS2450.h"

// Address of the DS2450 quad a/d (lsb first)
uint8_t DS2450_address[] = { 0x20, 0x27, 0xBF, 0x00, 0x00, 0x00, 0x00, 0xFC };

//Wind direction globals
// Define North offset (0 to 15) here.
// To determine the offset, point the vane North.
// If the readout says WSW, then count how many steps to get to N
// So from WSW, to W, WNW all the way to N is 5 steps.
const int NORTH_OFFSET = 5;

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

bool windDirectionFlag = false;  //Flag true with new reading
int direction = 16;

OneWire ow(ONE_WIRE_BUS);
DS2450 ds2450(&ow, DS2450_address);


//--------------------------- Temperature ------------------------------------
#include <DallasTemperature.h>

// Pass our oneWire reference to Dallas Temperature library
DallasTemperature wsDS18b20(&oneWire);

// Address of the DS18B20 in the AAG Windstation
DeviceAddress windStation = {0x10, 0x74, 0x4E, 0x1B, 0x00, 0x08, 0x00, 0x2E};

float tempC;
float tempF;
bool temperatureFlag = false;  //Flag true with new reading




//======================= setup() ===============================
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);
  ds2423.begin();       //Speed
  ds2450.begin();       //Direction
  wsDS18b20.begin();      //Temperature

  /*
    // Temperature debug stuff
    Serial.print("Found ");
    Serial.print(wsDS18b20.getDeviceCount(), DEC);
    Serial.println(" devices.");
    Serial.print("Parasite power is: ");
    if (wsDS18b20.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");
    Serial.print("windStation Address: ");
    printAddress(windStation);
    Serial.println();
  */

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  wsDS18b20.setResolution(windStation, 9);

  /*
    // Temperature debug stuff
    Serial.print("Device Resolution: ");
    Serial.print(wsDS18b20.getResolution(windStation), DEC);
    Serial.println();
  */

  //Start the TickTwo timers
  timer1.start();
  timer2.start();
  timer3.start();

  //Get initial values
  readWindSpeed();
  readWindDirection();
  wsTemperature();
}



//======================= loop() ===============================
void loop() {
  timer1.update(); 
  timer2.update(); 
  timer3.update(); 
  
  char buffer[50];
  if (windSpeedFlag) {
    windSpeedFlag = false;
    if (windSpeed < 120) {
      snprintf(buffer, sizeof(buffer), "%.2f°C, %.2f°F, %d MPH, %s", tempC, tempF, windSpeed, directions[direction]);
      Serial.println(buffer);
    }
  }
  if (windDirectionFlag) {
    windDirectionFlag = false;
      snprintf(buffer, sizeof(buffer), "%.2f°C, %.2f°F, %d MPH, %s", tempC, tempF, windSpeed, directions[direction]);
    Serial.println(buffer);
  }
    if (temperatureFlag) {
    temperatureFlag = false;
      snprintf(buffer, sizeof(buffer), "%.2f°C, %.2f°F, %d MPH, %s", tempC, tempF, windSpeed, directions[direction]);
    Serial.println(buffer);
  }

}