#include "DpWiFi.h"
#include "Rgb.h"

/*
	Todo: Detect if we lose connection, and show disconnected error page
	Todo: Needs to detect when charging starts/ends and toggle the display/relay accordingly
*/

namespace DpWiFi{
	uint32_t last_read = 0;
	bool connected = false;	// internal

	uint32_t expiry_start = 0;		// When the last expiry update took place
	uint32_t expiry_dur = 0;		// Duration of expiry
	uint16_t COST_PER_HOUR = 60;	// Todo: Needs to be initialized by API

	// Returns time until expiry in whole seconds
	uint32_t chargeExpires(){

		const uint32_t ms = millis();
		if( !expiry_start || ms-expiry_start > expiry_dur )
			return 0;
		return (expiry_dur-(ms-expiry_start))/1000;

	}

	// Check if config button is pressed
	void check(){

		if( digitalRead(Config::PIN_CONFIG) == LOW ){

			Serial.println("Starting WiFi Manager");
			Rgb::setStatus(Rgb::STATUS_CONFMODE, true);
			
			DisplayManager::setPage(DisplayManager::PAGE_WIFI);
			WiFiManager wifiManager;
    		wifiManager.startConfigPortal("DogePower");
			DisplayManager::setPage(DisplayManager::PAGE_RUNTIME);

			Rgb::setStatus(Rgb::STATUS_CONFMODE, false);
			reconnect();

		}

	}

	bool isConnected(){
		return ( WiFi.status() == WL_CONNECTED );
	}

	void reconnect(){

		WiFi.mode(WIFI_STA);
		if( !WiFi.enableSTA(true) ){
			Serial.println("WiFi not setup");
		}

	}

	void setup(){
		
		WiFi.begin();
		pinMode(Config::PIN_CONFIG, INPUT);
		check();
		reconnect();
		// Connect

	}
	void loop(){
		
		check();
		
		const uint32_t ms = millis();
		const bool con = isConnected();
		// Connection status changed
		if( con != connected ){

			connected = con;
			Rgb::setStatus(Rgb::STATUS_CONNECTION_FAIL, !isConnected());
			DisplayManager::setPage(con ? DisplayManager::PAGE_RUNTIME : DisplayManager::PAGE_ERROR);
			
		}

		if( con && (ms-last_read > READ_DLY || !last_read) ){
			last_read = ms;

			char fullURL[128];
			strcpy(fullURL, endpoint);
			strcat(fullURL, Config::addr);
			strcat(fullURL, "/");

			Serial.printf("Sending HTTP request to %S \n", fullURL);

			HTTPClient http;
			http.begin(fullURL);
			//http.begin("http://jsonplaceholder.typicode.com/todos/1");
			int httpCode = http.GET();
			Serial.printf("Got response code %i\n");
			if( httpCode > 0 ){

				StaticJsonDocument<2048> doc;
				String response = http.getString();
				Serial.println("HTTP Response: ");
				Serial.println(response.c_str());
				DeserializationError error = deserializeJson(doc, response);
				if( error ){
					Serial.print(F("deserializeJson() failed: "));
    				Serial.println(error.f_str());
					return;
				}
				uint32_t expiry = doc["seconds_left"];

				if( expiry && !expiry_start ){

					expiry_start = ms;
					expiry_dur = expiry*1000;
					DisplayManager::setPage(DisplayManager::PAGE_RUNTIME, true);
					Relay::toggle(true);

				}
				Serial.printf("Got expiry %i \n", expiry);

			}

		}

	}

}

