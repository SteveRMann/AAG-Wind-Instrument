#define SKETCH "windStation"
/*
     Wind Speed:
     Read a counter value from a DS2423.
     This counter value is mapped to actual windspeed.

     Wind Direction
     This function reads the wind vane direction from a
     DS2450 quad a/d in an AAG wind vane.

     Wind Speed and Wind Direction data is on a one-wire bus (D4).

     Temperature
     Humidity and temperature are from a DHT22 on D1

     Pyranometer:
     Reads the pyranometer value on A0 once a second.
       Connections:
       --------------
       White- +5V
       Green- Signal (A0)
       Clear- Ground

*/


//The PubSub constructor and HOSTPREFIX MUST be unique on the network.
#define HOSTPREFIX "WIND"

#include <Arduino.h>
#include <OneWire.h>
#include <TickTwo.h>

//------------------ WiFiMulti declarations ------------------
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

// WiFi credentials Defined in kaywinnet.h
#include "Kaywinnet.h"

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;

// setup_wifi vars
char macBuffer[24];       // Holds the last three digits of the MAC, in hex.
char hostNamePrefix[] = HOSTPREFIX;
char hostName[12];        // Holds hostNamePrefix + the last three bytes of the MAC address.


// ------------------------- For MQTT -------------------------
#include <ESP8266WiFi.h>        //Connect (and reconnect) an ESP8266 to the a WiFi network.
#include <PubSubClient.h>       //connect to a MQTT broker and publish/subscribe messages in topics.
#include "Kaywinnet.h"

//Declare an object of class WiFiClient, which allows to establish a connection to a specific IP and port
//Declare an object of class PubSubClient, which receives as input of the constructor the previously defined WiFiClient.
//The constructor MUST be unique on the network.
WiFiClient aagClient;
PubSubClient client(aagClient);

#define NODENAME "windStation"                             //Give this node a name
const char *cmndTopic = NODENAME "/cmnd";                  //Incoming commands, payload is a command.
const char *celciusTopic = NODENAME "/celsius";                  //payload is the temperature in Celsius.
const char *fahrenheitTopic = NODENAME "/fahrenheit";               //payload is the temperature in fahrenheit.
const char *humidityTopic = NODENAME "/humidity";                 //payload is the humidity in percent.
const char *windSpeedTopic = NODENAME "/speed";                    //payload is the wind speed in MPH.
const char *windDirectionTopic = NODENAME "/direction";                //payload is the wind direction (char array).
const char *pyroTopic = NODENAME "/pyranometer";              //payload is the pyranometer value.
const char *connectName =  NODENAME "1";                   //Must be unique on the network
const char *mqttServer = MQTT_SERVER;                      //Local broker defined in Kaywinnet.h
const int mqttPort = MQTT_PORT;


// ------------------------- Global variables -------------------------
byte celsius = 0;
byte fahrenheit = 0;
byte humidity = 0;
int pyroPin = A0;       //data pin for the Pyranometer
int pyroValue = 0;      //value coming from the Pyranometer
bool pyroFlag = false;  //True when there is a new reading



//---------------- Prototypes (Must precede TickTwo timers ---------------
// Must preceed ticker calls.
void readWindDirection();
void readWindSpeed();
void readDht22();
void readPyranometer();

TickTwo timer1(readWindSpeed, 10000);       //Get windspeed every ten seconds
TickTwo timer2(readWindDirection, 1000);    //Get wind direction every 1 second
TickTwo timer3(readDht22, 15000);           //Call myTemperature() every 15,000 ms.
TickTwo timer4(readPyranometer, 30000);     //Get pyro value every 30 seconds


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
#include <SimpleDHT.h>
int pinDHT22 = D1;
SimpleDHT22 dht22(pinDHT22);
bool temperatureFlag = false;  //Flag true with new reading


//--------------------------- Pyranometer ------------------------------------
void readPyranometer() {
  // read the value from the pyro:
  pyroValue = analogRead(pyroPin);
  pyroFlag = true;
}


//======================= setup() ===============================
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);
  ds2423.begin();       //Speed
  ds2450.begin();       //Direction
  setup_wifiMulti();

  //Call the setServer method on the PubSubClient object, passing as first argument the
  //address and as second the port.
  client.setServer(mqttServer, mqttPort);
  mqttConnect();

  //Show the topics:
  Serial.print(F("celciusTopic= "));
  Serial.println(celciusTopic);
  Serial.print(F("fahrenheitTopic= "));
  Serial.println(fahrenheitTopic);
  Serial.print(F("humidityTopic= "));
  Serial.println(humidityTopic);
  Serial.print(F("windSpeedTopic= "));
  Serial.println(windSpeedTopic);
  Serial.print(F("windDirectionTopic= "));
  Serial.println(windDirectionTopic);
  Serial.print(F("pyroTopic= "));
  Serial.println(pyroTopic);


  //Start the TickTwo timers
  timer1.start();
  timer2.start();
  timer3.start();
  timer4.start();

  //Get initial values
  readWindSpeed();
  readWindDirection();
  readDht22();          //Temperature and humidity

}




//======================= loop() ===============================
void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
  timer4.update();

  //Make sure we stay connected to the mqtt broker
  if (!client.connected()) {
    mqttConnect();
  }
  if (!client.loop()) {
    client.connect(connectName);
  }


  //  char buffer[50];
  if (windSpeedFlag) {
    windSpeedFlag = false;
    if (windSpeed < 120) {
      publsh();
    }
  }

  if (windDirectionFlag) {
    windDirectionFlag = false;
    publsh();
  }

  if (temperatureFlag) {
    temperatureFlag = false;
    publsh();
  }

  if (pyroFlag) {
    pyroFlag = false;
    publsh();
  }


}
void publsh() {
  char bufr[50];

  //    if (celsius < 200) {        //In case of sensor error, celcius returns 200


  itoa(celsius, bufr, 10);
  client.publish(celciusTopic, bufr);

  itoa(fahrenheit, bufr, 10);
  client.publish(fahrenheitTopic, bufr);

  itoa(humidity, bufr, 10);
  client.publish(humidityTopic, bufr);

  itoa(windSpeed, bufr, 10);
  client.publish(windSpeedTopic, bufr);

  client.publish(windDirectionTopic, directions[direction]);

  itoa(pyroValue, bufr, 10);
  client.publish(pyroTopic, bufr);   //Watts per square meter (W/m^2)


  //Print
  snprintf(bufr, sizeof(bufr), "%d°C, %d°F, %d%%, %d MPH, %s, %dW/m^2", celsius, fahrenheit, humidity, windSpeed, directions[direction], pyroValue);
  Serial.println(bufr);
}
