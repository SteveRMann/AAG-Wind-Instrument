/*
   dht.ino
   Function to read the temperature and humidity from a DHT11

   In the top of the main program:
   #include <SimpleDHT.h>
   int pinDHT11 = D1;
   SimpleDHT11 dht11(pinDHT11);

   Global variables (until I figure out how to return two values)
   byte celsius = 0;
   byte fahrenheit = 0;
   byte humidity = 0;

*/



void readDht11() {
  // Returns globals:
  //byte celsius
  //byte fahrenheit
  //byte humidity

  int err = SimpleDHTErrSuccess;

  if ((err = dht11.read(&celsius, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err));
    delay(1000);
    return;
  }

  fahrenheit = (celsius * 9) / 5 + 32;

  /*
    Serial.println(F("---- readDht() ----"));
    Serial.print((int)celsius);
    Serial.print("°C, ");
    Serial.print(fahrenheit);
    Serial.print("°F, ");
    Serial.print((int)humidity);
    Serial.println("%");
  */
  
  temperatureFlag = true;
  return;
}
