#include "Config.h"
#include "Relay.h"
#include "DpWiFi.h"
#include "DisplayManager.h"
#include "Rgb.h"

void setup(){

	Serial.begin(115200);
	Serial.println("IT BEGINS!");

	Rgb::setup();
	Rgb::setStatus(Rgb::STATUS_BOOTING, true);

	Config::setup();
	
	DpWiFi::setup();
	Relay::setup();
	DisplayManager::setup();

	Rgb::setStatus(Rgb::STATUS_BOOTING, false);
	
	if( !DpWiFi::isConnected() )
		DisplayManager::setPage(DisplayManager::PAGE_ERROR);

}


void loop(){

	Config::loop();
	DpWiFi::loop();
	DisplayManager::loop();
	Relay::loop();
	Rgb::loop();


}
