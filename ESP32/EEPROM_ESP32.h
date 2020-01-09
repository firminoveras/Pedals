#ifndef _EEPROM_ESP32_h
#define _EEPROM_ESP32_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
//#include "WProgram.h"
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
