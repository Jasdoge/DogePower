#ifndef __DisplayManager
#define __DisplayManager
#include "Config.h"
#include <Arduino.h>
#include <SPI.h>
#include "FS.h"
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include <PNGdec.h>
#include "DpWiFi.h"



namespace DisplayManager{

	extern uint8_t page;
		const uint8_t PAGE_BOOT = 0;
		const uint8_t PAGE_WIFI = 1;
		const uint8_t PAGE_RUNTIME = 2;		// Operating as normal
		const uint8_t PAGE_ERROR = 4;
	extern uint8_t subPage;

	const uint16_t SCREEN_WIDTH = 320;
	const uint16_t SCREEN_HEIGHT = 240;

	const uint16_t TEXT_COLOR = 0xB4E6;

	extern uint32_t ticker;

	extern TFT_eSPI tft;
	/*
	extern TFT_eSprite *spr[8];
	extern TFT_eSprite beerBar;
	extern TFT_eSprite beerBg;
	*/
	extern TFT_eSprite *toDraw;	// Sprite to draw to in callback
	
	

	extern PNG png;
	extern File pngfile;
	extern int16_t xpos;
	extern int16_t ypos;

	void setPage( uint8_t p, uint8_t force = false );
	void drawPNG(char *url, uint16_t x, uint16_t y);
	void drawPNGSprite(TFT_eSprite *sprite, char *url);
	void * pngOpen( const char *filename, int32_t *size );
	void pngClose(void *handle);
	int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length);
	int32_t pngSeek(PNGFILE *page, int32_t position);
	void pngDrawToScreen( PNGDRAW *pDraw );
	void pngDrawToSprite( PNGDRAW *pDraw );

	void drawQrCode( char *address, const uint16_t xPos, const uint16_t yPos, const uint16_t color = 0, const uint8_t w = 7 );

	void setup();
	void loop();

	

}



#endif
