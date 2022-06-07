#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;

//-------------------- setup --------------------
void setup()
{
  Serial.begin(115200);
  Serial.println();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  Serial.println(F("Status\tHumidity (%)\tTemperature (C)\t(F)\t\tHeatIndex (C)\t(F)"));

  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);
  // use this instead: 
  dht.setup(D1, DHTesp::DHT22); // Connect DHT sensor to D1
}


//-------------------- loop --------------------
void loop(){
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print(F("\t"));
  Serial.print(humidity, 1);
  Serial.print(F("%\t\t"));
  Serial.print(temperature, 1);
  Serial.print(F("°\t\t"));
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print(F("°\t\t"));
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("%\t\t");
  Serial.print(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
  Serial.println("°");
  delay(2000);
}
