#include "driver/video.h"
#include "driver/vga.h"

struct video_info video_info;

void init_vesa()
{
	
	uint32_t *vram_addr;
	int vram;
	//直接从地址获取
	video_info.color_number = *((uint16_t *)VIDEO_INFO_ADDR);
	video_info.wide = (uint32_t )(*((uint16_t *)(VIDEO_INFO_ADDR+2)));
	video_info.high = (uint32_t )(*((uint16_t *)(VIDEO_INFO_ADDR+4)));
	
	//先获取地址，在转换成指针
	vram_addr = (uint32_t *)(VIDEO_INFO_ADDR+6);
	video_info.vram = (uint8_t *) (*vram_addr);
	
	put_str("\n");
	put_int(video_info.color_number);
	put_str("\n");
	put_int(video_info.wide);
	put_str("\n");
	put_int(video_info.high);
	put_str("\n");
	put_int((int )*vram_addr);
	put_str("\n");
}

