#define SKETCH "pyranometer"

/*
   Sketch to read the SP-215 pyranometer on A0
*/

#include <TickTwo.h>

//Globals
int pyroPin = A0;       //select the data pin for the Pyranometer
int pyroValue = 0;      //the value coming from the Pyranometer
bool pyroFlag = false;  //True when there is a new reading


//---------------- Prototypes (Must precede TickTwo timers ---------------
// Must preceed ticker calls.
void readPyranometer();

TickTwo timer4(readPyranometer, 1000);    //Get pyro value every 1 second

void readPyranometer() {
  // read the value from the pyro:
  pyroValue = analogRead(pyroPin);
  pyroFlag = true;
}


//--------------- setup ---------------
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(SKETCH);

  //Start the TickTwo timers
  timer4.start();
}


//--------------- loop ---------------
void loop() {
  timer4.update();

  if (pyroFlag) {
    pyroFlag = false;
    Serial.println(pyroValue);
  }
}
