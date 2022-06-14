#define SKETCH "dht11.ino"
#include <SimpleDHT.h>

int pinDHT11 = D1;
SimpleDHT11 dht11(pinDHT11);


//------------------------- setup -------------------------
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(F(SKETCH));
}


//------------------------- loop -------------------------
void loop() {
  // start working...

  // read without samples.
  byte celsius = 0;
  byte fahrenheit = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  
  if ((err = dht11.read(&celsius, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err));
    delay(1000);
    return;
  }

  fahrenheit = (celsius * 9) / 5 + 32;

  Serial.print((int)celsius);
  Serial.print("°C, ");
  Serial.print(fahrenheit);
  Serial.print("°F, ");
  Serial.print((int)humidity);
  Serial.println("%");


  // DHT11 sampling rate is 1HZ.
  delay(1000);
}
