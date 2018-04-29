/**
@system.c
@auther: hzc
@date 2017/11/25
system task
*/

#include "gui/gui.h"
#include "gui/graphic.h"
#include "gui/layer.h"

#include "kernel/io.h"
#include "driver/cmos.h"
#include "kernel/task.h"
#include "kernel/syscall.h"
#include "kernel/system.h"

//left 4+16+4=24
#define BAR_RIGHT_LEFT 24
#define BAR_CHAR_WIDE 8

void task_system_entry()
{
	struct task *task = task_current();
	
	int x, y;
	x = layer_desktop->wide - BAR_RIGHT_LEFT;
	y = 3;
	int font_color = COLOR_WHITE230;
	int bc_color = COLOR_BLACK30;
	
	for(;;){
		sys_milliDelay(100);
		//time
		draw_square16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*16, y, 8*6,16, bc_color);
		draw_int16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*16 , y, font_color, get_hour_hex());
		draw_string16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*14, y, font_color, ":");
		draw_int16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*13, y, font_color, get_min_hex());
		
		//date
		draw_square16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*10, y, BAR_CHAR_WIDE*10,16, bc_color);
		draw_int16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*10 , y, font_color, get_year());
		draw_string16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*6, y, font_color, "/");
		
		draw_int16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*5 , y, font_color, get_mon_hex());
		draw_string16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*3, y, font_color, "/");
		draw_int16(layer_desktop->buf,layer_desktop->wide, x - BAR_CHAR_WIDE*2, y, font_color, get_day_of_month());
		
		
		layer_refresh(layer_desktop, x - BAR_CHAR_WIDE*16, y, x,y+16);
	}
}
