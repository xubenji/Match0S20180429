#include "driver/vga.h"
#include "lib/string.h"
#include "kernel/io.h"

struct vga VGA;
void init_vga()
{
	VGA.vram = (int8_t *)VGA_VRAM;
	VGA.cursor_pos = 0;
	VGA.color = 0x07;
	clean_screen();
	put_str(">init vga\n");
}

void put_char(uint8_t ch)
{
	VGA.cursor_pos = get_cursor();

	if(ch == 0xd){				//回车CR(0x0d)
		VGA.cursor_pos = line_feed(VGA.cursor_pos);
	}else if(ch == 0xa){		//换行LF(0x0a)
		VGA.cursor_pos = line_feed(VGA.cursor_pos);
	}else if(ch == 0x8){		//退格BS(backspace)
		VGA.cursor_pos = backspace(VGA.cursor_pos);
	}else{						//一般字符
		VGA.vram[VGA.cursor_pos*2] = ch;
		VGA.vram[VGA.cursor_pos*2+1] = VGA.color;
		VGA.cursor_pos++;
	}
	
	if(VGA.cursor_pos >= 80*25){
		roll_screen();
	}
	set_cursor(VGA.cursor_pos);
}

void put_str(uint8_t *str)
{
	while(*str != 0x00){
		put_char(*str);
		str++;
	}
}

void put_int(uint32_t num)
{
	int8_t str_ptr[16];
	itoa(str_ptr, num);
	put_str(str_ptr);
}

uint16_t get_cursor()
{
	uint16_t pos_low, pos_high;		//设置光标位置的高位的低位
	//取得光标位置
	io_out8(0x03d4, 0x0e);			//光标高位
	pos_high = io_in8(0x03d5);
	io_out8(0x03d4, 0x0f);			//光标低位
	pos_low = io_in8(0x03d5);
	
	return (pos_high<<8 | pos_low);	//返回合成后的值
}

uint16_t set_cursor(uint16_t cursor_pos)
{
	//设置光标位置 0-2000
	io_out8(0x03d4, 0x0e);			//光标高位
	io_out8(0x03d5, VGA.cursor_pos>>8);
	io_out8(0x03d4, 0x0f);			//光标低位
	io_out8(0x03d5, VGA.cursor_pos);
}

void roll_screen()
{
	int x, y;
	for(y = 0; y < 24; y++){
		for(x = 0; x < 80; x++){
			VGA.vram[(y*80 + x)*2] = VGA.vram[((y+1)*80 + x)*2];
			VGA.vram[(y*80 + x)*2 + 1] = VGA.vram[((y+1)*80 + x)*2 + 1];
		}
	}
	for(x = 0; x < 80; x++){
		VGA.vram[(24*80 + x)*2] = ' ';
		VGA.vram[(24*80 + x)*2 + 1] = 0x07;
	}
	VGA.cursor_pos = 80*24;
}

void clean_screen()
{
	int x, y;
	for(y = 0; y < 24; y++){
		for(x = 0; x < 80; x++){
			VGA.vram[(y*80 + x)*2] = ' ';
			VGA.vram[(y*80 + x)*2 + 1] = 0x07;
		}
	}
	VGA.cursor_pos = 0;
	set_cursor(0);
}

uint16_t line_feed(uint16_t cursor_pos)
{
	//换行
	uint16_t words_line = cursor_pos%80;
	cursor_pos -= words_line;
	cursor_pos += 80;
	return cursor_pos;
}

uint16_t backspace(uint16_t cursor_pos)
{
	uint16_t old_cursor_pos = cursor_pos;
	
	old_cursor_pos--;
	VGA.vram[old_cursor_pos*2] = ' ';
	VGA.vram[old_cursor_pos*2+1] = 0X07;
	
	return old_cursor_pos;
}

void set_color(uint8_t color)
{
	VGA.color = color;
}

void reset_color()
{
	VGA.color = 0x07;
}
