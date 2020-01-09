#ifndef _EEPROM_ESP32_h
#define _EEPROM_ESP32_h

#ifndef _ARDUINO_H
#include <Arduino.h>
#define _ARDUINO_H
#endif

class EEPROM_ESP32{
public:
	void clear_EEPROM();
	void setEEPROM_SSID(String SSID);
	void setEEPROM_PASS(String PASS);
	void setEEPROM_AUTOCONNECT(bool AUTOCONNECT);
	bool getEEPROM_AUTOCONNECT();
	bool hasEEPROM_DATA();
	String getEEPROM_PASS();
	String getEEPROM_SSID();
};

extern EEPROM_ESP32 Flash;
#endif
