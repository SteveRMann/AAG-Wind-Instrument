/*
     Direction
     Original code: https://github.com/jbechter/arduino-onewire-DS2450

     This function reads the wind vane direction from a
     DS2450 quad a/d in an AAG wind vane.

*/

void readWindDirection() {
  ds2450.update();
  if (ds2450.isError()) {
    Serial.println("Error reading from DS2450 device");
  } else {
    //int direction = 16;
    float ch_a = ds2450.getVoltage(0);
    float ch_b = ds2450.getVoltage(1);
    float ch_c = ds2450.getVoltage(2);
    float ch_d = ds2450.getVoltage(3);

    //debug
    //Serial.print(ch_a); Serial.print(", ");
    //Serial.print(ch_b); Serial.print(", ");
    //Serial.print(ch_c); Serial.print(", ");
    //Serial.print(ch_d); Serial.println(", ");

    
    for (int i = 0; i < 16; i++) {
      if (((ch_a < directionTable[i][0] + 1.0) && (ch_a >= directionTable[i][0] - 1.0)) &&
          ((ch_b < directionTable[i][1] + 1.0) && (ch_b >= directionTable[i][1] - 1.0)) &&
          ((ch_c < directionTable[i][2] + 1.0) && (ch_c >= directionTable[i][2] - 1.0)) &&
          ((ch_d < directionTable[i][3] + 1.0) && (ch_d >= directionTable[i][3] - 1.0))) {
        direction = (i + NORTH_OFFSET) % 16;
        break;
      }
    }
    //Serial.println(directions[direction]);
    windDirectionFlag = true;
  }
}
