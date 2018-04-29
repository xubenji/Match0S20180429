#include "command/command.h"

void builtin_os()
{
	//清除光标
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);
	//换一行
	console_newline();
	//显示提示信息
	draw_string16(console.buf, console.wide, console.cursor_x,console.cursor_y, console.char_color, "Match OS.");
	layer_refresh(console.layer,4,console.cursor_y, console.wide + -4 ,console.cursor_y + 16);
	//换一行
	console_newline();
}
