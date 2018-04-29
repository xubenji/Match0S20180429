#include "command/command.h"

void builtin_help()
{
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);	
	console_string("\n");
	//命令占12个字符，后面加解析
	console_string("[command]   [function]\n");
	console_string("os          @Inbuilt Display system information\n");
	console_string("cls         @Inbuilt Empty console\n");
	console_string("ls          @Inbuilt List file\n");
	console_string("mem         @Inbuilt Get memory infomation\n");
	/*console_string("walk        @Game Walk man\n");
	console_string("2048        @Game 2048\n");
	console_string("test        @Soft Api test\n");
	console_string("cpuid       @Soft Shouw CPUID\n");*/
}
