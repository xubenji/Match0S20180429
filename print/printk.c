#include "print/vsprintf.h"
#include "kernel/syscall.h"
#include "gui/gui.h"

int printk(const char *fmt, ...)
{
	int i;
	char buf[256];

	va_list arg = (va_list)((char*)(&fmt) + 4); /*4是参数fmt所占堆栈中的大小*/
	i = vsprintf(buf, fmt, arg);
	sys_print(buf, i);

	return i;
}
//gui printk
int gprintk(const char *fmt, ...)
{
	int i;
	char buf[256];
	memset(buf, 0, 256);
	va_list arg = (va_list)((char*)(&fmt) + 4); /*4是参数fmt所占堆栈中的大小*/
	i = vsprintf(buf, fmt, arg);
	sys_gprint(buf, i);

	return i;
}
