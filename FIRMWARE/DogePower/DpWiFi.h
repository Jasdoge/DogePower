#ifndef __DpWiFi
#define __DpWiFi
#include <Arduino.h>
#include "Config.h"
#include "Rgb.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include "DisplayManager.h"

namespace DpWiFi{

	extern uint32_t last_read;

	bool isConnected();
	void reconnect();
	void check();
	void setup();
	void loop();

}

#endif
