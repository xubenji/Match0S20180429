#include "gui/graphic.h"
#include "kernel/types.h"
#include "gui/font.h"
#include "gui/layer.h"
#include "kernel/memory.h"
#include "driver/vga.h"
#include "driver/video.h"

uint8_t draw_num_buf[32];
uint8_t draw_num_ptr = 0;

void draw_pix16(uint16_t *vram,uint32_t wide, uint32_t x, uint32_t y, uint16_t color)
{
	uint16_t *ram;
	
	ram = (uint16_t *)(vram + ((y * wide) + x) );
	/*判定是否在边界内，如果是就可以直接写数据*/
	if(x >= 0 && x < video_info.wide && y >= 0 && y < video_info.high){
		*ram = color;
	}
}

void draw_square16(uint16_t *vram,uint32_t wide, uint32_t x, uint32_t y, uint32_t w,uint32_t h, uint16_t color)
{
	/*wide-video wide*/
	uint32_t x1, y1;
	for (y1 = 0; y1 < h; y1++) {
		for (x1 = 0; x1 < w; x1++) {
			draw_pix16(vram, wide, x + x1,y + y1, color);
		}
	}
}

void draw_word16(uint16_t *vram, uint32_t wide, uint32_t x, uint32_t y , uint16_t color, uint8_t *ascii)
{
	int i;
	char d /* data */;
	for (i = 0; i < 16; i++) {
		d = ascii[i];
		if ((d & 0x80) != 0) { draw_pix16(vram, wide, x + 0, y + i, color); }
		if ((d & 0x40) != 0) { draw_pix16(vram, wide, x + 1, y + i, color); }
		if ((d & 0x20) != 0) { draw_pix16(vram, wide, x + 2, y + i, color); }
		if ((d & 0x10) != 0) { draw_pix16(vram, wide, x + 3, y + i, color); }
		if ((d & 0x08) != 0) { draw_pix16(vram, wide, x + 4, y + i, color); }
		if ((d & 0x04) != 0) { draw_pix16(vram, wide, x + 5, y + i, color); }
		if ((d & 0x02) != 0) { draw_pix16(vram, wide, x + 6, y + i, color); }
		if ((d & 0x01) != 0) { draw_pix16(vram, wide, x + 7, y + i, color); }
	}
	return;
}

void draw_a_word16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint8_t *s)
{
	
	draw_word16(vram, wide, x, y, color, standard_font + *s * 16);
	x += 8;
	return;
}

void draw_string16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y, uint16_t color, uint8_t *s)
{
	for (; *s != 0x00; s++) {
		draw_word16(vram, wide, x, y, color, standard_font + *s * 16);
		x += 8;
	}
	return;
}

int debug_x = 0,debug_y = 16;
int oy = 16;

int draw_string16_debug(uint16_t *vram, uint32_t wide,uint16_t color, uint8_t *s)
{
	int lines = 0;
	oy = debug_y;
	for (; *s != 0x00; s++) {
		if(*s == '#'){	//#是特殊字符用于换行
				debug_y += 16;
				debug_x = 0;
				s++;
				lines++;
		}else {	//#是特殊字符用于换行，不显示
			draw_word16(vram, wide, debug_x, debug_y, color, standard_font + *s * 16);
			debug_x += 8;
			if(debug_x > 800){
				debug_x = 0;
				debug_y += 16; 
			}
		}
		if(debug_y > 600){
			debug_y = 0;
		}
	}
	return lines+2;
}



void draw_hex16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint32_t value) {
	char ch[8];
	int i = 8;
	/**
	*char *s = "0123456789abcdef";
	*ch[i] = s[(value>>(i*4))&0xf];
	*/
	while (i-- > 0) {
		ch[i] = "0123456789abcdef"[(value>>(i*4))&0xf];
	}
	/*
	draw_string16(vram, wide, x,y,color, "0x");	
	x += 8*2;
	*/
	//循环显示
	for(i = 7; i >= 0; i--){
		draw_a_word16(vram, wide, x,y,color, &ch[i]);	
		x += 8;
	}
}

void draw_num(uint32_t value, uint32_t base) {
	uint32_t n = value / base;
	int r = value % base;
	if (r < 0) {
		r += base;
		--n;
	}
	if (value >= base)
		draw_num(n, base);
	draw_num_buf[draw_num_ptr++] = "0123456789"[r];
}

void draw_int16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint32_t value) {
	int i;
	draw_num_ptr = 0;
	
	/*清空缓冲区*/
	for(i = 0; i < 32; i++){
		draw_num_buf[i] = 0;
	}
	
	draw_num(value, 10);
	for(i = 0; i <=7; i++){
		draw_a_word16(vram, wide, x,y,color, &draw_num_buf[i]);	//e
		x += 8;
	}
	draw_num_ptr = 0;
}

int draw_circle16(uint16_t *vram,int wide, uint32_t center_x,uint32_t center_y, uint32_t radius,uint16_t color)  
{
    int x, y, p;  
    x = 0, y = radius, p = 1-radius;  

    while (x < y)
    {
        draw_pix16(vram, wide, center_x + x, center_y + y, color);  
        draw_pix16(vram, wide, center_x - x, center_y + y, color);  
        draw_pix16(vram, wide, center_x - x, center_y - y, color);  
        draw_pix16(vram, wide, center_x + x, center_y - y, color);  
        draw_pix16(vram, wide, center_x + y, center_y + x, color);  
        draw_pix16(vram, wide, center_x - y, center_y + x, color);  
        draw_pix16(vram, wide, center_x - y, center_y - x, color);  
        draw_pix16(vram, wide, center_x + y, center_y - x, color);  
  
        x++;
        if (p < 0) p += 2*x + 1;  
        else  
        {  
            y--;  
            p += 2*x - 2*y + 1;  
        }  
    }
    return 1;  
}

/* 
绘制一个窗口 
通过所给屏幕大小推断出窗口所有的大小
*/

void window_draw(uint16_t *buf,int wide, int high, char *title, char activate)  
{
	int title_len;
	int j, i;
	char close_button_element;
	uint16_t color_window, color_button, color_border;
	uint16_t close_button_color;
	if(!activate){	// 0
		color_button = COLOR_GREY170;
		color_border = COLOR_GREY;
	}else{
		color_button = COLOR_BLACK;
		color_border = COLOR_WHITE;
	}
	
	//绘制整个窗口
    draw_square16(buf,wide, 0, 0, wide,high, COLOR_GREY170);
	
	//绘制活动窗口
	draw_square16(buf,wide, 0, 0, wide,1, color_border);
	draw_square16(buf,wide, 0, 0, 1,high, color_border);
	draw_square16(buf,wide, wide-1, 0, 1,high, color_border);
	draw_square16(buf,wide, 0, high-1, wide,1, color_border);
	
	draw_square16(buf,wide, 4, 24, wide-8,high - 4 - 24, COLOR_WHITE);
	//绘制标题
	title_len = strlen(title);
	draw_string16(buf,wide, wide - 4 - title_len*8 - 8, 4, COLOR_BLACK, title );
	//绘制标题按钮
	static char close_button[15][15] = {
		
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,},
		{0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,},
		{0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,},
		{0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,},
		{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,},
		{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,},
		{0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,},
		{0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,},
		{0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,},
		{0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		
	};
	for(j = 0; j < 15; j++){
		for(i = 0; i < 15; i++){
			close_button_element = close_button[j][i];
			if(close_button_element == 0){
				close_button_color = COLOR_GREY170;
			}else if(close_button_element == 1){
				close_button_color = color_button;
			}
			buf[(j + 4 +1) * wide + (4 + i + 1)] = close_button_color;
		}
	}
	
	
	
} 
/* 
绘制一个窗口 
通过所给屏幕大小推断出窗口所有的大小
*/

void draw_block(uint16_t *buf,int wide, int high, char activate)  
{
	uint16_t color_window, color_border;
	if(!activate){	// 0
		color_window = COLOR_GREY170;
		color_border = COLOR_GREY;
	}else{
		color_window = COLOR_GREY;
		color_border = COLOR_WHITE;
	}

	//绘制整个窗口
    draw_square16(buf,wide, 0, 0, wide,high, color_window);
	
	//绘制活动窗口
	draw_square16(buf,wide, 0, 0, wide,1, color_border);
	draw_square16(buf,wide, 0, 0, 1,high, color_border);
	draw_square16(buf,wide, wide-1, 0, 1,high, color_border);
	draw_square16(buf,wide, 0, high-1, wide,1, color_border);
	
	draw_square16(buf,wide, 4, 4, wide-8,high - 8, COLOR_WHITE);
}


void draw_taskbar(uint16_t *buf,int wide, int high, int type)  
{
	uint16_t color_window;
	
	if(type){	//down
		color_window = COLOR_GREY170;
		//绘制整个窗口
		draw_square16(buf,wide, 0, 0, wide,high, color_window);
		
	}else{	//up
		color_window = COLOR_BLACK30;
		//绘制整个窗口
		draw_square16(buf,wide, 0, 0, wide,high, color_window);
	}
	
}


void draw_line16(uint16_t *buf,int wide, int x0, int y0, int x1, int y1, uint16_t color)
{
	int i, x, y, len, dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	
	x = x0 << 10;
	y = y0 << 10;
	
	if(dx < 0){
		dx = -dx;
	}
	if(dy < 0){
		dy = -dy;
	}
	if(dx >= dy ){
		len = dx + 1;
		if(x0 > x1){
			dx = -1024;
		} else {
			dx = 1024;
			
		}
		if(y0 <= y1){
			dy = ((y1 - y0 + 1) << 10)/len;
		} else {
			dy = ((y1 - y0 - 1) << 10)/len;
		}
		
		
	}else{
		len = dy + 1;
		if(y0 > y1){
			dy = -1024;
		} else {
			dy = 1024;
			
		}
		if(x0 <= x1){
			dx = ((x1 - x0 + 1) << 10)/len;
		} else {
			dx = ((x1 - x0 - 1) << 10)/len;
		}	
	}
	for(i = 0; i < len; i++){
		buf[(y >> 10)*wide + (x >> 10)] = color;
		x += dx;
		y += dy;
	}
}
