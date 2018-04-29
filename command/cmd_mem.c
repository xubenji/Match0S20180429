#include "command/command.h"

void builtin_mem()
{
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);	
	console_string("\nMemory size: ");
	console_int(memory_total_size/(1024*1024));		//显示MB
	console_string("        MB");
	console_string("\nMemory free: ");
	console_int(get_free_memory()/(1024*1024));		//显示MB
	console_string("        MB\n");
}


