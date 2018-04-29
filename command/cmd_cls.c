#include "command/command.h"

void builtin_cls()
{
	draw_square16(console.buf, console.wide, 4,4, console.wide - 8, console.high - 4 - 4, console.background_color);
	layer_refresh(console.layer,4,4, console.wide -4,4+160);
	console.cursor_x = 4;
	console.cursor_y = 4;
}
