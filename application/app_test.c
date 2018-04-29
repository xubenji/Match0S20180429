#include "application/application.h"
#include "driver/console.h"
#include "kernel/memory.h"

/*WALK*/
void app_test()
{
	WINDOW window;
	int key;
	int16_t *window_buf;
	char *hd_buf;
	
	window_buf = (int16_t *)malloc(320* 200*2);
	hd_buf = (char *)malloc(512);
	
	window = creatWindow(window_buf, 320, 200, 0xf0f0, "appTest");
	
	fillColor(COLOR_RED);
	drawSquare(window, 4, 32, 100, 50);
	fontColor(COLOR_PURPLE);
	drawString(window, 4, 32, "Hello appTest");
	
	fillColor(COLOR_BLUE);
	drawPoint(window, 5, 33);

	drawSquare(window, 100, 32, 100, 50);
	
	refreshWindow(window, 4, 40, 50, 100);
	
	drawLine(window, 4, 40, 50, 100);
	
	drawHex(window, 100, 60, 0xa5a5);
	fillColor(COLOR_WHITE);

	readSector(0, hd_buf, 1);
	
	drawHex(window, 100, 60+16, hd_buf[0]);
	hd_buf[0] = 0x99;
	writeSector(0, hd_buf, 1);
	
	for(;;){
		milliDelay(100);
		
		if(getChar() == 'q'){
			break;
		}
	}
	mfree(window_buf);
	closeWindow(window);
	closeTask();
}