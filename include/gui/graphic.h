#ifndef _GRAPHIC_H
#define	_GRAPHIC_H
#include "kernel/types.h"

//0x1f=31 3f=63 1f=31

/*通过把24位的rgb转换后变成16位对应颜色*/
#define RGB16(r, g, b) (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (r >> 3))

#define COLOR_WHITE RGB16(0xff, 0xff, 0xff)
#define COLOR_BLACK RGB16(0x00, 0x00, 0x00)
#define COLOR_RED RGB16(200, 10, 10)
#define COLOR_YELLOW RGB16(0xff, 0xff, 0x00)
#define COLOR_GREEN RGB16(0x00, 0xff, 0x00)
#define COLOR_BLUE RGB16(0x00, 0x00, 0xff)
#define COLOR_BLUE RGB16(0x00, 0x00, 0xff)
#define COLOR_PURPLE RGB16(128, 0x00, 0xff)
#define COLOR_TAUPE RGB16(0x80, 0x80, 0x80)
#define COLOR_GREY RGB16(0xc0, 0xc0, 0xc0)
#define COLOR_GREY170 RGB16(170, 170, 170)

#define COLOR_WHITE150 RGB16(150, 150, 150)
#define COLOR_WHITE200 RGB16(200, 200, 200)
#define COLOR_WHITE230 RGB16(230, 230, 230)
#define COLOR_BLACK30 RGB16(30, 30, 30)


#define RGB16_L8(c) (c & 0xff)
#define RGB16_H8(c) ((c & 0xff00) >> 8)












/**
所有的都面向一个地址来操作。
*/
/*16bit 绘制*/
void draw_pix16(uint16_t *vram,uint32_t wide, uint32_t x, uint32_t y, uint16_t color);
void draw_square16(uint16_t *vram,uint32_t wide, uint32_t x, uint32_t y, uint32_t w,uint32_t h, uint16_t color);
void draw_word16(uint16_t *vram, uint32_t wide, uint32_t x, uint32_t y , uint16_t color, uint8_t *ascii);
void draw_string16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint8_t *s);
void draw_a_word(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint8_t *s);

void draw_hex16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint32_t value);
void draw_int16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint32_t value);
void draw_binary16(uint16_t *vram, uint32_t wide, uint32_t x,uint32_t y,uint16_t color, uint32_t value);
int draw_circle16(uint16_t *vram,int wide, uint32_t center_x,uint32_t center_y, uint32_t radius,uint16_t color);
void draw_line16(uint16_t *buf,int wide, int x0, int y0, int x1, int y1, uint16_t color);

void window_draw(uint16_t *buf,int wide, int high, char *title, char activate);


void draw_block(uint16_t *buf,int wide, int high, char activate);
void draw_taskbar(uint16_t *buf,int wide, int high, int type);
int draw_string16_debug(uint16_t *vram, uint32_t wide, uint16_t color, uint8_t *s);

#endif
