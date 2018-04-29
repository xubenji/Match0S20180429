#ifndef _VGA_H
#define _VGA_H
#include "kernel/types.h"

#define VGA_VRAM 0x800b8000

/* VGA */
#define	CRTC_ADDR_REG	0x3D4	/* CRT Controller Registers - Addr Register */
#define	CRTC_DATA_REG	0x3D5	/* CRT Controller Registers - Data Register */
#define	START_ADDR_H	0xC	/* reg index of video mem start addr (MSB) */
#define	START_ADDR_L	0xD	/* reg index of video mem start addr (LSB) */
#define	CURSOR_H	0xE	/* reg index of cursor position (MSB) */
#define	CURSOR_L	0xF	/* reg index of cursor position (LSB) */
#define	V_MEM_BASE	0x800b8000	/* base of color video memory */
#define	V_MEM_SIZE	0x8000	/* 32K: B8000H -> BFFFFH */

struct vga {
	int8_t *vram;
	uint16_t cursor_pos;
	int8_t color;
	
};

extern struct vga VGA;
void init_vga();
void put_char(uint8_t ch);
void put_str(uint8_t *str);
void put_int(uint32_t num);

uint16_t get_cursor();
uint16_t set_cursor(uint16_t cursor_pos);
uint16_t line_feed(uint16_t cursor_pos);
uint16_t backspace(uint16_t cursor_pos);
void set_color(uint8_t color);
void reset_color();

void roll_screen();
void clean_screen();

#endif

