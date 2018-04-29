#include "application/application.h"

/*Snack*/
void app_cpuid()
{
	WINDOW window;
	int key;
	int16_t *window_buf;
	unsigned int tsc_start_high, tsc_start_low;
	unsigned int tsc_end_high, tsc_end_low;
	unsigned int clock;
	
	char name_string[50];
	unsigned int family, model, stepping;
	
	window_buf = (int16_t *)malloc(360* 240*2);
	window = creatWindow(window_buf, 360, 240, 0xf0f0, "Match Cpuid");
	fontColor(COLOR_GREEN);
	
	fillColor(COLOR_WHITE);

	cpuCpuid(name_string, &family, &model, &stepping);
	
	drawString(window, 4, 27+16*3, name_string);
	
	drawString(window, 4, 27, "Family = ");
	drawInt(window, 4+8*10, 27, family);
	drawString(window, 4, 27+16, "Model = ");
	drawInt(window, 4+8*10, 27+16, model);
	drawString(window, 4, 27+16*2, "Stepping = ");
	drawInt(window, 4+8*10, 27+16*2, stepping);
	
	cpuRdtsc(&tsc_start_high, &tsc_start_low);

	for(;;){
		milliDelay(1000);
		cpuRdtsc(&tsc_end_high, &tsc_end_low);
		clock = (tsc_end_low - tsc_start_low) / 1000 / 1000;
		
		drawSquare(window, 4, 150, 200, 16);
		drawString(window, 4, 150, "Clock = ");
		drawInt(window, 4+8*10, 150, clock);
		drawString(window, 4+8*10+8*8, 150, "MHz");
		
		tsc_start_high = tsc_end_high;
		tsc_start_low = tsc_end_low;
		if(getChar() == 'q'){
			break;
		}
	}
	mfree(window_buf);
	closeWindow(window);
	closeTask();
}