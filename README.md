# AAG Wind Instrument
I acquired a brand new AAG Wind Instrument that had virtually no documentation.   Most of the links on the web were broken, so I had little to go on.
The schematic that I found is almost exactly the same as my device and was enough information to get started.

I also found Arduino libraries for the three DS devices on the PCB at https://github.com/jbechter.

<img src="[https://github.com/favicon.ico](https://github.com/SteveRMann/AAG-Wind-Instrument/image.jpg)" width="480">


## Versions
(From http://sheepdogsoftware.co.uk/sc3wmw.htm#gotoVer)
The wind speed/ direction sensor is "Version 3". Earlier versions used different electronics for sensing the wind direction and You can tell one from the other by
--the markings on the PCB:"Ceprei version 2" (Yes: two)
or
--By using an i2c Scanner. If there is a DS2450 (ID ends hex 20), the device is version 3.

## Getting Chip ID's
Each 1-wire chip has it's own unique 64-bit serial number. The program needs to know the serial numbers. I wrote an Arduino sketch "one_wire_address_finder" which revelaled three DS chips on the 1-wire network. 
The DS2450 (1-Wire Quad A/D Converter) handles the wind direction
The DS18S20 (1-Wire Temperature Digital Serial Sensor) provides the temperature. (Works just like an 18B20, but 4X the cost).
And the DS2423 (4kbit 1-Wire RAM with Counter) determines the wind speed.

Sketch: *one_wire_address_finder* found these chips on my instrument:
- address : 0x2027BF00000000FC  //DS2450 4-channel (ADC, A/D converter)
- address : 0x10744E1B0008002E  //DS1920, DS1820, DS18S20 (Temperature with alarm trips)
- address : 0x1DF6F90000000056  //DS2423 (4Kb NV RAM memory with external counters)





## DS2450_Direction
Original code: https://github.com/jbechter/arduino-onewire-DS2450


This example demonstrates the use of the DS2450 library and the Arduino
OneWire library to read wind vane direction from a Dallas Semiconductor DS2450
quad a/d in an AAG wind vane.



![Schematic](https://user-images.githubusercontent.com/8091425/170117819-a2d5b2f6-9e48-4b12-897f-a5d291664eb4.jpg)
