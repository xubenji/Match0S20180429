#ifndef _VIDEO_H
#define _VIDEO_H
#include "kernel/types.h"

#define VIDEO_INFO_ADDR 0x80006100 
/*
	VIDEO_INFO_ADDR = vodeo color 
	VIDEO_INFO_ADDR+2 = vodeo wide
	VIDEO_INFO_ADDR+4 = vodeo high
	VIDEO_INFO_ADDR+6 = vodeo ram
	
*/

struct video_info {
	uint16_t color_number;		//2字节
	uint32_t wide, high;	//2字节+2字节
	uint8_t *vram;				//指针
};

extern struct video_info video_info;

void init_vesa(void);

#endif

