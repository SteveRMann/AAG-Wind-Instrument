/*
//========================== Read the temperature ==========================
// Returns globals tempC and tempF
void wsTemperature() {
  wsDS18b20.requestTemperatures();      //Issue a global temperature request
  printTemperature(windStation);      //Print out the data
}


// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  tempC = wsDS18b20.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  tempF = DallasTemperature::toFahrenheit(tempC); // Converts tempC to Fahrenheit
  
  //  Serial.print("Temp C: ");
  //  Serial.print(tempC);
  //  Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
  //  Serial.println(tempF);

  temperatureFlag = true;
}





// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
*/
