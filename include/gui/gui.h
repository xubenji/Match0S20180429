#ifndef _GUI_H
#define	_GUI_H
#include "kernel/types.h"
#include "gui/graphic.h"
#include "gui/layer.h"


void init_gui();
extern struct layer *layer_desktop, *layer_mouse, *layer_win, *layer_console;
extern struct layer *layer_taskbar_up;

int height;


struct layer *creat_layer(int type, int x, int y, int wide, int high);
void draw_moving_window(uint16_t *buf, int wide, int high, int16_t bc);

int sys_gprint(char* buf, int len);
#endif
