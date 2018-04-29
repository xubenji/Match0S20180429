#include "gui/gui.h"
#include "gui/layer.h"
#include "gui/graphic.h"
#include "kernel/memory.h"
#include "driver/vga.h"
#include "driver/video.h"
#include "driver/console.h"
#include "kernel/ards.h"
#include "driver/mouse.h"
#include "gui/desktop.h"


struct layer_manage *layer_manage;

struct layer *layer_desktop, *layer_mouse, *layer_console, *layer_taskbar_up, *layer_taskbar_down;
uint16_t *buf_desktop, *buf_mouse, *buf_console, *buf_taskbar_up, *buf_taskbar_down;

struct layer *layer_pic;
uint16_t *buf_pic;

extern int height;

void init_gui()
{
	//初始化图形管理
	layer_manage = layer_manage_init((uint16_t *)video_info.vram, video_info.wide, video_info.high);
	
	layer_desktop = layer_alloc(LAYER_TYPE_NOTMOVE);
	layer_mouse = layer_alloc(LAYER_TYPE_MOVE);
	layer_console = layer_alloc(LAYER_TYPE_WINDOW);
	layer_pic = layer_alloc(LAYER_TYPE_NOTMOVE);
	
	
	
	//layer_taskbar_up = layer_alloc(LAYER_TYPE_NOTMOVE);
	//layer_taskbar_down = layer_alloc(LAYER_TYPE_NOTMOVE);
	
	buf_desktop = sys_malloc(video_info.wide * video_info.high * 2);
	buf_mouse = sys_malloc(16*16*2);
	buf_console = sys_malloc(CONSOLE_WIN_WIDE*CONSOLE_WIN_HIGH*2);
	buf_pic = sys_malloc(320*240*2);
	
	
	//buf_taskbar_up = sys_malloc(TASKBAR_UP_HIGH*video_info.wide*2);
	//buf_taskbar_down = sys_malloc(TASKBAR_DOWN_HIGH*video_info.wide*2);
	
	
	layer_init(layer_desktop, buf_desktop, video_info.wide, video_info.high, LAYER_NOINVISIBLE);
	layer_init(layer_mouse, buf_mouse, 16, 16, LAYER_INVISIBLE);
	layer_init(layer_console, buf_console, CONSOLE_WIN_WIDE, CONSOLE_WIN_HIGH ,LAYER_NOINVISIBLE);
	//初始化图层信息，LAYER_NOINVISIBLE说这个图层没有透明部分 LAYER_INVISIBLE有透明部分
	layer_init(layer_pic, buf_pic, 320, 240 ,LAYER_NOINVISIBLE);
	
	
	
	
	init_desktop();
	//BC
	draw_square16(layer_desktop->buf,layer_desktop->wide, 0, 0, layer_desktop->wide,layer_desktop->high, COLOR_BLACK);
	
	
	draw_square16(layer_desktop->buf,layer_desktop->wide, 0, 0, layer_desktop->wide,TASKBAR_UP_HIGH, COLOR_BLACK30);
	
	draw_square16(layer_desktop->buf,layer_desktop->wide, 0, layer_desktop->high-TASKBAR_DOWN_HIGH, layer_desktop->wide,TASKBAR_DOWN_HIGH, COLOR_WHITE150);
	
	draw_block(layer_console->buf,layer_console->wide, layer_console->high, 0);
	
	//在里面绘制内容
	draw_square16(layer_pic->buf,layer_pic->wide, 0, 0, layer_pic->wide,layer_pic->high, COLOR_RED);
	
	
	image_cursor(layer_mouse->buf, LAYER_INVISIBLE);

	
	layer_shift(layer_desktop,0,0);
	layer_shift(layer_console,200+(layer_desktop->wide-CONSOLE_WIN_WIDE)/2,(layer_desktop->high-CONSOLE_WIN_HIGH)/2);
	layer_shift(layer_mouse,layer_desktop->wide/2,layer_desktop->high/2);
	//设置在屏幕中的位置
	layer_shift(layer_pic,200,100);
	
	
	layer_height(layer_desktop,0);
	//layer_height(layer_taskbar_up,-1);
	//layer_height(layer_taskbar_down,2);
	height = -1;
	layer_height(layer_console,1);
	
	layer_height(layer_pic,height);
	layer_height(layer_mouse,3);
	
	/*
	uint32_t *a = sys_malloc(16);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 0, COLOR_WHITE, (uint32_t )a);
	sys_mfree(a);
	uint32_t *b = sys_malloc(16);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*1, COLOR_WHITE, (uint32_t )b);
	uint32_t *c = sys_malloc(32);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*2, COLOR_WHITE, (uint32_t )c);
	
	uint32_t *d = sys_malloc(64);
	sys_mfree(c);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*3, COLOR_WHITE, (uint32_t )d);
	
	uint32_t *e = sys_malloc(64);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*4, COLOR_WHITE, (uint32_t )e);
	sys_mfree(e);
	uint32_t *f = sys_malloc(64);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*5, COLOR_WHITE, (uint32_t )f);
	
	
	uint32_t *g = sys_malloc(128);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*6, COLOR_WHITE, (uint32_t )g);
	sys_mfree(g);
	uint32_t *h = sys_malloc(128);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*7, COLOR_WHITE, (uint32_t )h);
	uint32_t *i = sys_malloc(256);
	uint32_t *j = sys_malloc(256);
	uint32_t *k = sys_malloc(512);
	uint32_t *o = sys_malloc(1024);
	
	
	
	
	
	
	
	
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*8, COLOR_WHITE, (uint32_t )i);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*9, COLOR_WHITE,(uint32_t )j);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*10, COLOR_WHITE, (uint32_t )k);
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 16*11, COLOR_WHITE, (uint32_t )o);
	
	*/
	
	
	
	
	
	layer_refresh(layer_desktop, 0, 0, layer_desktop->wide, layer_desktop->high);
	layer_refresh(layer_console, 0, 0, layer_console->wide, layer_console->high);
	//layer_refresh(layer_taskbar_up, 0, 0, layer_taskbar_up->wide, layer_taskbar_up->high);
	//layer_refresh(layer_taskbar_down, 0, 0, layer_taskbar_down->wide, layer_taskbar_down->high);
	//刷新图层
	layer_refresh(layer_pic, 0, 0, layer_pic->wide, layer_pic->high);
	layer_refresh(layer_mouse, 0, 0, layer_mouse->wide, layer_mouse->high);
	
}
static char cursor[16][16] = {
		{1,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0},
		{1,2,1,0,0,0,0,0,0,0,0,0},
		{1,2,2,1,0,0,0,0,0,0,0,0},
		{1,2,2,2,1,0,0,0,0,0,0,0},
		{1,2,2,2,2,1,0,0,0,0,0,0},
		{1,2,2,2,2,2,1,0,0,0,0,0},
		{1,2,2,2,2,2,2,1,0,0,0,0},
		{1,2,2,2,2,2,2,2,1,0,0,0},
		{1,2,2,2,2,2,2,2,2,1,0,0},
		{1,2,2,2,2,1,1,1,1,1,1,0},
		{1,2,2,2,1,0,0,0,0,0,0,0},
		{1,2,2,1,0,0,0,0,0,0,0,0},
		{1,2,1,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0},
};
	
void image_cursor(uint16_t *buf, int16_t bc)
{
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == 1) {
				buf[y * 16 + x] = COLOR_BLACK;
			}
			if (cursor[y][x] == 2) {
				buf[y * 16 + x] = COLOR_WHITE;
			}
			if (cursor[y][x] == 0) {
				buf[y * 16 + x] = bc;
			}
		}
	}
}

void draw_moving_window(uint16_t *buf, int wide, int high, int16_t bc)
{
	int x, y;
	for (y = 0; y < high; y++) {
		for (x = 0; x < wide; x++) {
			buf[y*wide+x] = bc;
		}
	}
	draw_square16(buf,wide, 0, 0, wide,2,RGB16(220, 220, 220));
	draw_square16(buf,wide, 0, 0, 2,high,RGB16(220, 220, 220));
	draw_square16(buf,wide, wide-2, 0, 2,high,RGB16(220, 220, 220));
	draw_square16(buf,wide, 0, high-2, wide,2,RGB16(220, 220, 220));
}
extern int oy;
int sys_gprint(char* buf, int len)
{
	char* p = buf;
	int i = len;
	int lines = draw_string16_debug(layer_desktop->buf, layer_desktop->wide, COLOR_GREEN, buf);
	layer_refresh(layer_desktop, 0, 0, layer_desktop->wide, oy+lines*16);
	
	return 0;
}


