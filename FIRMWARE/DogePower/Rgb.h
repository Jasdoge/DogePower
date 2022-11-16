#ifndef __Rgb
#define __Rgb
#include <Arduino.h>
#include "Config.h"

namespace Rgb{

	extern uint8_t statuses;	// Holds bitwise status flags
		const uint8_t STATUS_BOOTING = 0x1;				// booting up the device
		const uint8_t STATUS_CONFMODE = 0x2;			// Config mode is active, connect to WiFi
		const uint8_t STATUS_CONNECTION_FAIL = 0x4;		// Connection failed

	void setStatus( uint8_t status, bool on );
	void setup();
	void loop();

}

#endif
