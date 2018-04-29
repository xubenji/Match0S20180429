#include "syscall/printf.h"
#include "syscall/syscall.h"

int printf(const char *fmt, ...)
{
	int i;
	char buf[256];

	va_list arg = (va_list)((char*)(&fmt) + 4); /*4是参数fmt所占堆栈中的大小*/
	i = vsprintf(buf, fmt, arg);
	printf(buf, i);

	return i;
}
