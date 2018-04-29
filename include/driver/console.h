#ifndef _CONSOLE_H
#define _CONSOLE_H
#include "kernel/types.h"
#include "lib/fifo.h"
#include "gui/gui.h"

#define CONSOLE_FIFO_BUF_LEN 128
#define CONSOLE_CHAR_ROWS 25		//
#define CONSOLE_CHAR_CLUMS 80		//

#define CONSOLE_WIN_HIGH (CONSOLE_CHAR_ROWS*16+8)
#define CONSOLE_WIN_WIDE (CONSOLE_CHAR_CLUMS*8+8)

#define MAX_COMMAND_LEN 126

struct console
{
	uint32_t fifo_buf[CONSOLE_FIFO_BUF_LEN];
	uint16_t *buf;			//图形缓冲区
	int wide, high;
	uint16_t cursor_x, cursor_y, cursor_color;
	uint16_t columns, rows;
	uint16_t background_color, char_color;
	struct FIFO32 fifo;		//输入输出缓冲区
	struct layer *layer;		//输入输出缓冲区
	
};
extern struct console console;

void task_console_entry();	//任务入口
void console_newline();		//换行操作
void console_char(int ch, char move);	//控制台显示字符
void console_string(char *s);	//控制台显示字符串
void console_command_prompt();	//显示提示符
void console_tip(char *str);	//提示信息
void console_hex(int hex);		//控制台显示16进制数
void console_int(int num);		//控制台显示10进制数

#endif

