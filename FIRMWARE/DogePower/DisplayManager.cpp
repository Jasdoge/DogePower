#include "DisplayManager.h"
#include "qrcoderm.h"

namespace DisplayManager{

	uint8_t page;
	TFT_eSPI tft = TFT_eSPI();

	PNG png;
	File pngfile;
	int16_t xpos = 0;
	int16_t ypos = 0;
	uint32_t ticker = 0;
	
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

			const uint32_t expiry = DpWiFi::chargeExpires();
			// QR
			if( !expiry ){
				drawPNG("/payment_screen.png", 0, 0);
				drawQrCode( Config::addr, 102, 62, 0, 4 );

				tft.setTextColor(TEXT_COLOR);
				tft.setTextDatum(MC_DATUM);
				char out[9];
				itoa(DpWiFi::COST_PER_HOUR, out, 10);
				tft.drawString(out,SCREEN_WIDTH/2,206,2);

			}
			// Clock
			else{
				drawPNG("/socket_active.png", 0, 0);
			}
			
		}
		else if( p == PAGE_BOOT ){

			tft.setTextColor(TFT_WHITE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("Booting",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}
		else if( p == PAGE_WIFI ){

			tft.setTextColor(TFT_WHITE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("WiFi Screen",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}else if( p == PAGE_ERROR ){

			tft.fillScreen(TFT_RED);
			tft.setTextColor(TFT_WHITE);
			tft.setTextDatum(MC_DATUM);
			tft.drawString("WiFi Screen",SCREEN_WIDTH/2,SCREEN_HEIGHT/2,2);
			
		}
		

	}

	void drawQrCode( char *address, const uint16_t xPos, const uint16_t yPos, const uint16_t color, const uint8_t w ){

		const uint8_t blocks = 29;	// 29 squares in each direction
		const uint8_t buffer = 3;

		QRCode qrcode;
		uint8_t qrcodeBytes[qrcode_getBufferSize(buffer)];

		// Todo: build address
		char full[42];
		strcpy(full, "doge:");
		strcat(full, address);

		qrcode_initText(&qrcode, qrcodeBytes, buffer, ECC_MEDIUM, full);
		for( uint8_t y = 0; y < qrcode.size; ++y ){

			const uint16_t startY = yPos+w*y;

			for( uint8_t x = 0; x < qrcode.size; ++x ){

				const uint16_t startX = xPos+w*x;

				if( qrcode_getModule(&qrcode, x, y) )
					tft.fillRect(startX, startY, w, w, color);

			}

		}

	}

	void setup(){

		tft.init();
		tft.setRotation(1);
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

		const uint32_t ms = millis();

		if( ms-ticker < 1000 )
			return;
		ticker = ms;

		if( page == PAGE_RUNTIME ){
			
			if( DpWiFi::chargeExpires() ){
				
				// Todo: Update clock.
				
			}

		}

	}


}


