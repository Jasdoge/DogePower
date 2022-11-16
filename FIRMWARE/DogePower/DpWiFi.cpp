#include "DpWiFi.h"
#include "Rgb.h"

/*
	Todo: Detect if we lose connection, and show disconnected error page
*/

namespace DpWiFi{
	uint32_t last_read = 0;
	bool connected = false;	// internal

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
		

		const bool con = isConnected();
		// Connection status changed
		if( con != connected ){

			connected = con;
			Rgb::setStatus(Rgb::STATUS_CONNECTION_FAIL, !isConnected());
			DisplayManager::setPage(con ? DisplayManager::PAGE_RUNTIME : DisplayManager::PAGE_ERROR);
			
		}


	}
}

