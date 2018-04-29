#include "command/command.h"

void builtin_hd()
{
	draw_square16(console.buf, console.wide, console.cursor_x,console.cursor_y, 8, 16, console.background_color);
	layer_refresh(console.layer,console.cursor_x,console.cursor_y, console.cursor_x + 8,console.cursor_y + 16);	
	console_string("\nHD information");

	console_string("\nDisk numbers:");
	console_int(hd.devices);
	console_string("\nDisk size:");
	console_int(hd.sectors*512/(1024*1024));
	console_string("        MB");
	
	console_string("\n");
}
