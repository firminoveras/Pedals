#include "EEPROM_ESP32.h"
#include "EEPROM.h"

EEPROM_ESP32 Flash;

void EEPROM_ESP32::clear_EEPROM(){
	EEPROM.begin(64);
	for(int i = 0; i < 64; i++) EEPROM.writeChar(i + 1, 255);
	EEPROM.commit();
	EEPROM.end();
}

void EEPROM_ESP32::setEEPROM_SSID(String SSID){
	EEPROM.begin(64);
	for(int i = 0; i < 32; i++) EEPROM.writeChar(i + 1, 255);
	for(int i = 0; i < SSID.length(); i++) if(i + 1 < 32)EEPROM.writeChar(i + 1, SSID.charAt(i));
	EEPROM.commit();
	EEPROM.end();
}

void EEPROM_ESP32::setEEPROM_PASS(String PASS){
	EEPROM.begin(64);
	for(int i = 33; i < 64; i++) EEPROM.writeChar(i, 255);
	for(int i = 0; i < PASS.length(); i++) if(i < 32)EEPROM.writeChar(i + 33, PASS.charAt(i));
	EEPROM.commit();
	EEPROM.end();
}

void EEPROM_ESP32::setEEPROM_AUTOCONNECT(bool AUTOCONNECT){
	EEPROM.begin(64);
	EEPROM.write(0, (AUTOCONNECT ? '1' : '0'));
	EEPROM.commit();
	EEPROM.end();
}

bool EEPROM_ESP32::getEEPROM_AUTOCONNECT(){
	bool AUTOCONNECT;
	EEPROM.begin(64);
	AUTOCONNECT = EEPROM.read(0) == '1';
	EEPROM.end();
	return AUTOCONNECT;
}

bool EEPROM_ESP32::hasEEPROM_DATA(){	
	EEPROM.begin(64);
	bool hasData = EEPROM.read(1) != 255;
	EEPROM.end();
	return hasData;
}

String EEPROM_ESP32::getEEPROM_PASS(){
	String PASS = "";
	EEPROM.begin(64);
	for(int i = 33; i < 64; i++) if(EEPROM.read(i) != 255)PASS += (char)EEPROM.read(i);
	EEPROM.end();
	return PASS;
}

String EEPROM_ESP32::getEEPROM_SSID(){
	String SSID = "";
	EEPROM.begin(64);
	for(int i = 1; i < 32; i++) if(EEPROM.read(i) != 255)SSID += (char)EEPROM.read(i);
	EEPROM.end();
	return SSID;
}

