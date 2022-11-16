#include "DisplayManager.h"

namespace DisplayManager{

	uint8_t page;
	TFT_eSPI tft = TFT_eSPI();

	PNG png;
	File pngfile;
	int16_t xpos = 0;
	int16_t ypos = 0;
	
	TFT_eSprite *toDraw;	// Sprite to draw to in callback

	void * pngOpen( const char *filename, int32_t *size ){
		//Serial.printf("Attempting to open %s\n", filename);
		pngfile = LittleFS.open(filename, "r");
		*size = pngfile.size();
		return &pngfile;
	}

	void pngClose(void *handle) {
		File pngfile = *((File*)handle);
		if( pngfile )
			pngfile.close();
	}

	int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length) {
		if( !pngfile )
			return 0;
		page = page; // Avoid warning
		return pngfile.read(buffer, length);
	}

	int32_t pngSeek(PNGFILE *page, int32_t position) {
		if( !pngfile )
			return 0;
		page = page; // Avoid warning
		return pngfile.seek(position);
	}

	void pngDrawToScreen( PNGDRAW *pDraw ){
		uint16_t lineBuffer[SCREEN_WIDTH];
		png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
		tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
	}
	void pngDrawToSprite( PNGDRAW *pDraw ){
		uint16_t lineBuffer[SCREEN_WIDTH];
		png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
		toDraw->pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
	}

	void drawPNG( char *url, uint16_t x, uint16_t y){

		xpos = x;
		ypos = y;
		// Draw from filesys
		Serial.println(url);
		int16_t rc = png.open(url, pngOpen, pngClose, pngRead, pngSeek, pngDrawToScreen);
		if( rc == PNG_SUCCESS ){
			tft.startWrite();
			Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
			rc = png.decode(NULL, 0);
			png.close();
		}
		else{
			Serial.println("^ Failed to open");
		}
		tft.endWrite();

	}
	void drawPNGSprite( TFT_eSprite *sprite, char *url){
		toDraw = sprite;
		int16_t rc = png.open(url, pngOpen, pngClose, pngRead, pngSeek, pngDrawToSprite);
		Serial.println(url);
		if( rc == PNG_SUCCESS ){

			sprite->startWrite();
			rc = png.decode(NULL, 0);
			png.close();

		}
		else
			Serial.println("Failed to open");
		sprite->endWrite();
		Serial.println(ESP.getFreeHeap());
	}

	void setPage( uint8_t p, uint8_t force ){
		
		if( p == page && !force )
			return;
		page = p;

		tft.fillScreen(TFT_BLACK);
		if( p == PAGE_RUNTIME ){

			//drawPNG("/badge_bg.png", 0, 0);
			tft.setTextColor(TFT_WHITE, TFT_BLUE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("Default Page",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}
		else if( p == PAGE_BOOT ){

			//drawPNG("/badge_bg.png", 0, 0);
			tft.setTextColor(TFT_WHITE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("Booting",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}
		else if( p == PAGE_WIFI ){

			//drawPNG("/badge_bg.png", 0, 0);
			tft.setTextColor(TFT_WHITE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("WiFi Screen",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}else if( p == PAGE_ERROR ){

			tft.fillScreen(TFT_RED);
			//drawPNG("/badge_bg.png", 0, 0);
			tft.setTextColor(TFT_WHITE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("WiFi Screen",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}
		

	}

	void setup(){

		tft.init();
		tft.setRotation(2);
		tft.setTextDatum(MC_DATUM);
		
		setPage(PAGE_BOOT, true);

		// Load the sprites?
		Serial.println("Free heap:");
		Serial.println(ESP.getFreeHeap());

		/*
		beerBar.createSprite(15,1);
		drawPNGSprite(&beerBar, "/beer_bar.png");
		*/
	
	}

	void loop(){

		uint32_t ms = millis();

		if( page == PAGE_RUNTIME ){
			// Todo: Handle screensaver
		}

	}


}


