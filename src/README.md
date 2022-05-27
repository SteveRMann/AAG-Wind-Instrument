# Code in the src folder:

## DS2450Direction

### DS2450 Direction Original code: https://github.com/jbechter/arduino-onewire-DS2450
This example from jbetcher demonstrates the use of the DS2450 quad A/D library and the Arduino OneWire library to read wind vane direction.

## DS2423WindSpeed
This sketch uses the DS2423 counter chip to read a counter value from a Dallas Semiconductor DS2423.

### Datalogger
Writing data to an SD card. Was to be part of the DS2423WindSpeedCalibration program.

### DS2423WindSpeedCalibration
This sketch was used to get the accumulated count from the DS2423 over ten second windows.  The data was continuously displayed over the COM port.  While driving around the neighborhood, the count was correlated with the auto speed.  As expected, the count/speed line was linear, so a simple map function in the code provides the wind speed.

## Humidity Sensor
### HIH4030
This is a simple example sketch using the HIH4030.h library.

### HIH4030_Humidity_Sensor
This is an example sketch using the SparkFun_HIH4030.h library.

## windStation
Windstation is the merger of direction, windspeed and temperature codes.
