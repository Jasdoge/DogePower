#ifndef __Config
#define __Config
#include "FS.h"
#include <Arduino.h>
#include <LittleFS.h>

namespace Config{
	
	const uint8_t PIN_LED_G = 12;
	const uint8_t PIN_LED_R = 14;
	const uint8_t PIN_LED_B = 27;

	const uint8_t PIN_RELAY = 16;
	const uint8_t PIN_CONFIG = 26;

	extern char addr[36];

	void setup();
	void loop();
	void saveConfig();
	void loadConfig();

}



#endif
