#include "driver/console.h"
#include "kernel/memory.h"
#include "driver/vga.h"
#include "lib/fifo.h"
#include "gui/gui.h"
#include "driver/video.h"
#include "driver/mouse.h"
#include "kernel/syscall.h"
#include "command/command.h"
#include "application/application.h"


struct console console;

static char *host;
static int host_len;

//0X20-0X7E
void task_console_entry()
{
	struct task *task = task_current();
	int fifo_data;
	char command[MAX_COMMAND_LEN];
	uint8_t key_char[2];
	int x, y;
	
	fifo32_init(&console.fifo, CONSOLE_FIFO_BUF_LEN, console.fifo_buf);
	console.layer = layer_console;
	console.buf = console.layer->buf;
	console.wide = console.layer->wide;
	console.high = console.layer->high;

	
	console.cursor_x = 4;
	console.cursor_y = 4;
	console.cursor_color = COLOR_BLACK;
	console.background_color = COLOR_WHITE;
	console.char_color = COLOR_BLACK;
	
	host = "[host] ";
	host_len = strlen(host);
	
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
	
	
	//显示命令提示符
	draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, host);
	layer_refresh(console.layer,4,console.cursor_y, console.wide -4 ,console.cursor_y + 16);
	
	console.cursor_x += host_len*8;
	//绘制光标
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.cursor_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
	
	//init_application();
	
	for(;;){
		if(fifo32_status(&console.fifo) == 0){
			//delay(1);		//延迟
			//task_deprive(task, 1);		//夺取时间片
			
		} else {
			if(fifo32_status(&console.fifo) != 0){
				fifo_data = fifo32_get(&console.fifo);
				//console_display_character();
				key_char[0] = fifo_data;
				key_char[1] = 0;
				if(key_char[0] == '\b'){
					if(console.cursor_x > 4 + host_len*8){
						draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
						layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
	
						console.cursor_x -= 8;
					}
				}else if(key_char[0] == '\n'){
					draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
					layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
					
					//命令终止
					command[(console.cursor_x-8*host_len-4)/8] = 0;
					
					//读取命令
					if(strcmp(command, "os") == 0 || strcmp(command, "OS") == 0){
						builtin_os();
					}else if(strcmp(command, "cls") == 0 || strcmp(command, "CLS") == 0){
						builtin_cls();
					}else if(strcmp(command, "show") == 0 || strcmp(command, "SHOW") == 0){
						height=2;
						
						init_gui();
					}else if(strcmp(command, "help") == 0 || strcmp(command, "HELP") == 0){
						builtin_help();	
					}else if(strcmp(command, "ptask") == 0 || strcmp(command, "PTASK") == 0){
						builtin_ptask();
					}else if(strcmp(command, "mem") == 0 || strcmp(command, "MEN") == 0){
						builtin_mem();
					}else if(strcmp(command, "hd") == 0 || strcmp(command, "HD") == 0){
						builtin_hd();
					}else if(strcmp(command, "ie") == 0 || strcmp(command, "IE") == 0){
						builtin_ie();
				}else if(strncmp(command, "ls ",3) == 0 || strncmp(command, "ls",2) == 0){
						builtin_ls(command);
					}else if(command[0] != 0) {
						if(application_command(command) == 0){
							//清除光标
							draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
							layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
							//换一行
							console_newline();
							//显示提示信息
							draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, "Bad command.");
							layer_refresh(console.layer,4,console.cursor_y, console.wide + -4 ,console.cursor_y + 16);
							//换一行
							console_newline();
						}
						
					}else{
						console_newline();			
					}
					//清空命令行
					memset(command, 0, MAX_COMMAND_LEN);
					
					//显示命令提示符
					draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, host);
					layer_refresh(console.layer,4,console.cursor_y, console.wide -4 ,console.cursor_y + 16);
					
					console.cursor_x += host_len*8;
				}else {
					//清除背景
					draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
					layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
					//写上文字
					draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, key_char);
					layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
					
					//保存字符进入command
					command[(console.cursor_x-8*host_len-4)/8] = key_char[0];
					if(console.cursor_x < console.wide - 4 - 8){
						console.cursor_x += 8;	//向前走
						
					
					}
					
				}
				
				//显示光标
				draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.cursor_color);
				layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
	
			}
		}
	}
}

void console_newline()
{
	int x, y;
	if(console.cursor_y < 4 + (CONSOLE_CHAR_ROWS-1)*16){
		console.cursor_y += 16;	//换行
	}else {	
		//滚动
		for(y =  4; y < 4+16; y++){
			for(x = 4; x < console.wide-4; x++){
				console.buf[x + y*console.wide] = console.background_color; 
			}
		}
		for(y = 4; y < 4+CONSOLE_CHAR_ROWS*16-16; y++){
			for(x = 4; x < console.wide-4; x++){
				console.buf[x + y*console.wide] = console.buf[x + (y+16)*console.wide]; 
			}							
		}
		for(y =  4+CONSOLE_CHAR_ROWS*16 -16; y < 4+CONSOLE_CHAR_ROWS*16; y++){
			for(x = 4; x < console.wide-4; x++){
				console.buf[x + y*console.wide] = console.background_color; 
			}
		}
		//draw_square16(console.buf, console.wide, 4,console.high - 4-16, console.wide-8, 16, console.background_color);
		layer_refresh(console.layer,4,4, console.wide -4,4+CONSOLE_CHAR_ROWS*16);
	}
	console.cursor_x = 4;
}

void console_tip(char *str)
{
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);	
	console_string("\n");
	console_string(str);
	console_string("\n");
	draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, host);
	layer_refresh(console.layer,4,console.cursor_y, console.wide -4 ,console.cursor_y + 16);
	console.cursor_x += host_len*8;
	
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.cursor_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);	
}

void console_command_prompt()
{
	//显示命令提示符
	draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, host);
	layer_refresh(console.layer,4,console.cursor_y, console.wide -4 ,console.cursor_y + 16);
	console.cursor_x += host_len*8;
	
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.cursor_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);										
}


void console_char(int ch, char move)
{
	char s[2];
	s[0] = ch;
	s[1] = 0;
	if (s[0] == 0x0a) {	//enter
		console_newline();
	} else if (s[0] == 0x0d) {
		
	} else {//other
		
		draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, s);
		layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
		
		if (move != 0) {
			console.cursor_x += 8;
		}
	}
}

void console_string(char *s)
{
	for (; *s != 0; s++) {
		console_char(*s, 1);
	}
}

void console_hex(int hex)
{
	
	draw_hex16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, hex);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x +8*10 + 8,console.cursor_y + 16);
	console.cursor_x += 8*8;
}
void console_int(int num)
{
	draw_int16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, num);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x +8*10 + 8,console.cursor_y + 16);
	
}

