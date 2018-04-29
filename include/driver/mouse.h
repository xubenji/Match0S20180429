#ifndef _MOUSE_H
#define _MOUSE_H
#include "kernel/types.h"
#include "lib/fifo.h"

#define MOUSE_IRQ 12
#define	CASCADE_IRQ	2
#define	MOUSE_FIFO_BUF_LEN	256



struct mouse {
	unsigned char read_buf[3], phase;
	int mouse_fifo_buf[MOUSE_FIFO_BUF_LEN];
	int x_increase, y_increase, button;
	int x, y;
	int new_x, new_y;
	struct FIFO32 fifo;

};
extern struct mouse mouse;

void IRQ_mouse();
void mouse_handler(int32_t irq);
void init_mouse();
void enable_mouse(struct mouse *mouse);
int mouse_read(struct mouse *mouse, unsigned char data);

void task_mouse_entry();

void image_cursor(uint16_t *buf, int16_t bc);

#endif

