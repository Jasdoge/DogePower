#ifndef __DpWiFi
#define __DpWiFi
#include <Arduino.h>
#include "Config.h"
#include "Rgb.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include "DisplayManager.h"
#include "Relay.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

namespace DpWiFi{

	extern uint32_t last_read;
	extern uint32_t expiry_start;
	extern uint32_t expiry_dur;
	const uint32_t READ_DLY = 10e3;		// read every READ_DLY ms from the API
	extern uint16_t COST_PER_HOUR;		// Todo: Get from API

	const char endpoint[30] = "http://92.94.50.175/device/";

	uint32_t chargeExpires();			// returns seconds until charging expires, or 0 if it's off
	bool isConnected();
	void reconnect();
	void check();
	void setup();
	void loop();


}

#endif
