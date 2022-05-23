//======================= readWindSpeed() ===============================
void readWindSpeed() {
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
