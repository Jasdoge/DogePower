#include "Relay.h"

namespace Relay{

	void toggle( bool on ){
		digitalWrite(Config::PIN_RELAY, !on);
	}
	void setup(){
		pinMode(Config::PIN_RELAY, OUTPUT);
		toggle(false);
	}
	void loop(){

	}

}

