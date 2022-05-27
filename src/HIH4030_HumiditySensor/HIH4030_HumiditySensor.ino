#define SKETCH "HIH4030_HumiditySensor.ino"

/* **************************************************************************************
   HIH4030_HumiditySensor.ino

   Original code: https://github.com/sparkfun/SparkFun_HIH4030_Arduino_Library

   HH4030 functions:
     set supply voltage,
     define whether or not a temperature sensor is being used,
     define static temperature value in absense of a temperature sensor,
     and utilize the equations supplied by the manufacturer   to acquire accurate measurements of Relative Humidity (RH%).

   Hookup guide: https://learn.sparkfun.com/tutorials/hih-4030-humidity-sensor-hookup-guide

   Resources:
   Wire.h - Arduino I2C Library
   SparkFun_HIH4030.h - Arduino Library for SparkFun Humidity Sensor Breakout - HIH-4030

 ************************************************************************************** */


//--------------------------- Humidity ------------------------------------
#include "SparkFun_HIH4030.h"

//int tempAddress = 0x48;             // I2C: TMP102 Slave Address with ADD0 Grounded

// Are You Using a Temperature Sensor? 1 = YES / 0 = NO
int tempSensor = 1;

#define HIH4030_OUT A0              // Humidity sensor OUT
#define HIH4030_SUPPLY 5            // Supply Voltage - Typically 5 V

static float thOFFSET = 47.0;        //"Caliubration" offset to "real" humidity?

// Library Variables
HIH4030 sensorSpecs(HIH4030_OUT, HIH4030_SUPPLY);


//--------------------------- Temperature ------------------------------------
//#include <Wire.h>                   // For the temperature sensor
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature library
DallasTemperature wsDS18b20(&oneWire);

// Address of the DS18B20 on the Humidity Sensor board
DeviceAddress addressDS18b20 = {0x28, 0x39, 0xE5, 0x14, 0x01, 0x00, 0x00, 0xF0};

float tempC;
float tempF;
bool temperatureFlag = false;  //Flag true with new reading



//======================= setup() ===============================
void setup(void) {
  Serial.begin(115200);             // Serial Port
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);
  Serial.println();
  //  Wire.begin();                     // I2C: Utilized by TMP102 Temperature Sensor
  wsDS18b20.begin();                //Temperature
  wsDS18b20.setResolution(addressDS18b20, 9);  // set the resolution to 9 bit

}



//======================= loop() ===============================
void loop(void) {
  /*  IF tempSensor = 1 Utilizing a Temperature Sensor             */
  /*  IF tempSensor = 0 Utilizing a Static Value for Temperature   */
  if (tempSensor == 1) {
    dsTemperature();                // Get Temperature in °C
  } else {
    tempC = 32.0;                   // Static Temperature Value in C
  }

  printData(sensorSpecs, tempC);   // Print Sensor Readings
  Serial.println("");              // Return Space Between Readings
  delay(5000);                     // Slow Down Serial Output
}


//======================= printData() ===============================
void printData(HIH4030 sensor, float temperature) {
  static float ah = 0;
  static int i = 0;
  float th;
  static float h;


  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.print("°C (");
  //Serial.print((temperature * 9 / 5) + 32); Serial.println(" F)");
  //Serial.print((temperature * 9 / 5) + 32);
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.println(")°F");


  Serial.print("Sensor Voltage = ");
  Serial.print(sensor.vout());
  Serial.println(" V");

  Serial.print("Relative Humidity = ");
  Serial.print(sensor.getSensorRH());
  Serial.println(" %");

  Serial.print("True Relative Humidity = ");
  //Serial.print(sensor.getTrueRH(temperature));
  th = sensor.getTrueRH(temperature);
  Serial.print(th);
  Serial.println(" %");

  h = h + th;       //Accumulate total humidities
  i += 1;           //Bump humidity readings count
  ah = h / i;       //Calculate the average
  Serial.print (F("Average Humidity= "));
  Serial.print(ah);
  Serial.println(" %");

  Serial.print (F("Corrected TrueRH= "));
  Serial.print(th - thOFFSET);
  Serial.println(" %");

  


}
