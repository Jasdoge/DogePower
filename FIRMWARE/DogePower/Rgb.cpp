#include "Rgb.h"

namespace Rgb{

	uint8_t statuses = 0;

	void setStatus( uint8_t status, bool on ){
		statuses = statuses &~ status;
		if( on )
			statuses = statuses | status;

		uint8_t color = 0b000;
		// Highest priority last
		if( statuses & STATUS_BOOTING )
			color = 0b110;
		if( statuses & STATUS_CONNECTION_FAIL )
			color = 0b100;
		if( statuses & STATUS_CONFMODE )
			color = 0b001;
		
		digitalWrite(Config::PIN_LED_R, color&0b100 ? LOW : HIGH);
		digitalWrite(Config::PIN_LED_G, color&0b010 ? LOW : HIGH);
		digitalWrite(Config::PIN_LED_B, color&0b001 ? LOW : HIGH);

	}
	void setup(){

		pinMode(Config::PIN_LED_R, OUTPUT);
		pinMode(Config::PIN_LED_G, OUTPUT);
		pinMode(Config::PIN_LED_B, OUTPUT);

		// Active LOW
		digitalWrite(Config::PIN_LED_R, HIGH);
		digitalWrite(Config::PIN_LED_G, HIGH);
		digitalWrite(Config::PIN_LED_B, HIGH);

	}
	void loop(){

	}
}




