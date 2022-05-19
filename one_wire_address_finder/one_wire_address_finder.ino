// This sketch looks for 1-wire devices and
// prints their addresses (serial number) to
// the UART, in a format that is useful in Arduino sketches
// Tutorial:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

#define SKETCH "one_wire_address_finder"

#include <OneWire.h>

OneWire  ds(D4);  // Connect your 1-wire device to pin D4
byte addr[8];

void setup(void) {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Sketch: ");
  Serial.println(SKETCH);
  discoverOneWireDevices();
}

void getDeviceName() {
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x01:
      Serial.println("  //DS1990A, DS1990R, DS2401, DS2411 (1-Wire net address registration number)");
      break;
    case 0x02:
      Serial.println("  //DS1991 (Multikey iButton, 1152-bit secure memory)");
      break;
    case 0x04:
      Serial.println("  //DS1994, DS2404 (4Kb NV RAM memory and clock, timer, alarms)");
      break;
    case 0x05:
      Serial.println("  //DS2405 (Single addressable switch)");
      break;
    case 0x06:
      Serial.println("  //DS1993 (4Kb NV RAM memory)");
      break;
    case 0x08:
      Serial.println("  //DS1992 (1Kb NV RAM memory)");
      break;
    case 0x09:
      Serial.println("  //DS1982, DS2502 (1Kb EPROM memory)");
      break;
    case 0x0A:
      Serial.println("  //DS1995 (16Kb NV RAM memory)");
      break;
    case 0x0B:
      Serial.println("  //DS1985, DS2505 (16Kb EPROM memory)");
      break;
    case 0x0C:
      Serial.println("  //DS1996 (64Kb NV RAM memory)");
      break;
    case 0x0F:
      Serial.println("  //DS1986, DS2506 (64Kb EPROM memory)");
      break;
    case 0x10:
      Serial.println("  //DS1920, DS1820, DS18S20 (Temperature with alarm trips)");
      break;
    case 0x12:
      Serial.println("  //DS2406, DS2407 (1Kb EPROM memory, 2-channel addressable switch)");
      break;
    case 0x14:
      Serial.println("  //DS1971, DS2430A (256-bit EEPROM memory and 64-bit OTP register)");
      break;
    case 0x18:
      Serial.println("  //DS1963S (4K NVRAM memory and SHA–1 engine)");
      break;
    case 0x1A:
      Serial.println("  //DS1963L (4Kb NV RAM memory with write cycle counters)");
      break;
    case 0x1C:
      Serial.println("  //DS28E04-100 (4096-bit EEPROM memory, 2-channel addressable switch)");
      break;
    case 0x1D:
      Serial.println("  //DS2423 (4Kb NV RAM memory with external counters)");
      break;
    case 0x1F:
      Serial.println("  //DS2409 2-channel (addressable coupler for sub-netting)");
      break;
    case 0x20:
      Serial.println("  //DS2450 4-channel (ADC, A/D converter)");
      break;
    case 0x21:
      Serial.println("  //DS1921G, DS1921H, DS1921Z (Thermochron temperature logger)");
      break;
    case 0x22:
      Serial.println("  //DS1822 (Economy 1-Wire Digital Thermometer)");
      break;
    case 0x23:
      Serial.println("  //DS1973, DS2433 (4Kb EEPROM memory)");
      break;
    case 0x24:
      Serial.println("  //DS1904, DS2415 (RTC, Real-time clock)");
      break;
    case 0x26:
      Serial.println("  //DS2438 (Temperature, A/D)");
      break;
    case 0x27:
      Serial.println("  //DS2417 (RTC with interrupt)");
      break;
    case 0x28:
      if ((addr[6] != 0) || (addr[5] != 0)) {
        Serial.println(F("  //Fake DS18B20, ROM does not follow expected pattern 28-xx-xx-xx-xx-00-00-crc."));    
      } else {
        Serial.print("  //DS18B20 (Adjustable resolution temperature)");
      }
      break;
    case 0x29:
      Serial.println("  //DS2408 (8-channel addressable switch)");
      break;
    case 0x2C:
      Serial.println("  //DS2890 (1-channel digital potentiometer)");
      break;
    case 0x2D:
      Serial.println("  //DS1972, DS2431 (1024-bit, 1-Wire EEPROM)");
      break;
    case 0x30:
      Serial.println("  //DS2760 (Temperature, current, A/D)");
      break;
    case 0x33:
      Serial.println("  //DS1961S, DS2432 (1K EEPROM memory with SHA–1 engine)");
      break;
    case 0x37:
      Serial.println("  //DS1977 (Password-protected 32KB EEPROM)");
      break;
    case 0x3A:
      Serial.println("  //DS2413 (2-channel addressable switch)");
      break;
    case 0x41:
      Serial.println("  //DS1922L, DS1922T, DS1923, DS2422 (High-capacity Thermochron and Hygrochron loggers)");
      break;
    case 0x42:
      Serial.println("  //DS28EA00 (Programmable resolution digital thermometer with sequenced detection and PIO)");
      break;
    case 0x43:
      Serial.println("  //DS28EC20 20Kb (1-Wire EEPROM)");
      break;
    case 0x91:
      Serial.println("  //DS1981 (512–bit EPROM memory)");
      break;
    case 0x96:
      Serial.println("  //DS1955, DS1957B (Java–powered Cryptographic iButton)");
      break;
    default:
      Serial.println("  //Unknown device");
      return;
  }
}


void discoverOneWireDevices(void) {
  byte i;

  //Serial.println("Looking for 1 - Wire devices.");
  while (ds.search(addr)) {
    Serial.print("address : 0x");
    for ( i = 0; i < 8; i++) {
      //Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      Serial.print(addr[i], HEX);
      //      if (i < 7) {
      //      Serial.print(", ");
      //  }
    }
    if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
    }
    //Test the first byte
    getDeviceName();
    //Serial.println();
  }
  //Serial.print("\n\r\n\rThat's it.\r\n");
  ds.reset_search();
  return;
}

void loop(void) {
  // nothing to see here
}
