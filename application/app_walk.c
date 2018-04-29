#include "application/application.h"
#include "driver/console.h"
#include "kernel/memory.h"

/*WALK*/
void app_walk()
{
	WINDOW window;
	uint16_t *window_buf;
	int key;
	int x = 60, y = 50;
	//分配内存保存图层
	window_buf = (int16_t *)malloc(136* 96*2);
	//打开一个窗口
	window = creatWindow(window_buf, 136, 96, 0xf0f0, "Walk");
	//绘制主角
	
	fillColor(COLOR_GREEN);
	drawSquare(window, x, y, 8, 8);
	
	for(;;){
		milliDelay(10);
		key = getChar();
		
		if(key != -1){
			
			fillColor(COLOR_WHITE);
			drawSquare(window, x, y, 8, 8);
			if(key == 'q')break;
			if(key == 'a' && x > 4 + 8) x -= 4;
			if(key == 'd' && x < 136-8-8) x += 4;
			if(key == 'w' && y > 28+4) y -= 4;
			if(key == 's' && y < 92-4-8) y += 4;
			fillColor(COLOR_GREEN);
			drawSquare(window, x, y, 8, 8);
		}
	}
	mfree(window_buf);
	closeWindow(window);
	closeTask();
}
