# AAG Wind Instrument
I acquired a brand new AAG Wind Instrument that had virtually no documentation.   Most of the links on the web were broken, so I had little to go on.
The schematic that I found is almost exactly the same as my device and was enough information to get started.

I also found Arduino libraries for the three DS devices on the PCB at https://github.com/jbechter.

![image](https://user-images.githubusercontent.com/8091425/170121443-97abf312-1b1f-4ba7-961d-6e19dcfcacc2.jpg)


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


![Schematic](https://user-images.githubusercontent.com/8091425/170117819-a2d5b2f6-9e48-4b12-897f-a5d291664eb4.jpg)
The RJ11 green wire is the 1-wire bus, the red wire is ground.

## DS2450
### Wind Direction
The version 3 AAG wind direction sensor implements a delightfully clever way to encode the shaft's position.
The wind direction relies on 4 ADCs to sense the state of the 8 reed switches. No adjacent reed switches are on the same ADC. When no magnet is near one of the switches, the ADC it is connected to "sees" 5 volts. When a magnet is near one of the switches, the ADC "sees" either 0 volts or about 2.5 volts, depending on which switch the magnet is near. For a given ADC, only one switch can be closed at a given moment. Each ADC can "watch" two switches because resistors in the circuits allow the three possible voltages on each ADC monitored line.

Just to fill in the detail: When a switch is open, the input to the ADC is connected to 5v through a resistor. In one case, the reed switch connects the ADC directly to 0v, so when the switch closes, the ADC "sees" 0 volts. In the other case, the reed switch connects the ADC to 0 volts through a second resistor. This, in combination with the resistor connecting the ADC to 5 volts results in the ADC "seeing" about 2.5 volts.

Thus, the ADC "sees" a high, or low, or "medium" voltage.

## DS2423
### Wind Speed
Two magnets mounted on a second rotor attached to the wind cups axle operate a reed switch connected to the DS2423 (4kbit 1-Wire RAM with Counter) which provides a unique identification for this sensor with its 1-Wire serial number. One magnet is mounted in each of the two outermost holes of the rotor. This provides two counts per revolution which improves response at low wind speeds. It also provides rotational balance to the rotor, which becomes important with increasing wind speed, as the rotor can reach 2400 rpm in 160 km/h winds. The DS2423 keeps track of the total number of revolutions the wind cups make and provides the data to the bus master on demand. The chip contains two 232 bit counters and can be powered for ten years with a small Lithium battery, however, here power for the counter chip comes from D1, D2 and C1,  which form a half wave rectifier that “steals” power from the data line. The DS2423 can only be reset to zero when this “parasite power” is lost. Wind speed is calculated by the program taking the difference between two counts of the counter used. One count generated before and the other after a clocked interval. The output is currently given in rpm. This later needs to be converted to m/s or km/h.

