#include "Config.h"

namespace Config{
	
	char addr[36] = "2NCYsUzVmNBKJZAdZjeGdj2iu2ufhreZ34b";

	void saveConfig(){

		File file = LittleFS.open("/ADDR", FILE_WRITE);
		if( file ){

			Serial.printf("Saving addr %s\n", addr);
			file.print(addr);
			file.close();

		}

	}

	void loadConfig(){

		File file = LittleFS.open("/ADDR");
		if( file ){

			uint8_t i = 0;
			while( file.available() && i < 34 ){
				addr[i] = char(file.read());
				++i;
			}
			
			Serial.printf("Read addr: %s\n", addr);
			file.close();

		}

	}

	void setup(){

		LittleFS.begin(true);
		loadConfig();

	}

	void loop(){

	}


}
