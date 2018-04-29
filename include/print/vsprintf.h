#ifndef _VSPRINT_H
#define _VSPRINT_H

#include "kernel/types.h"

#define STR_DEFAULT_LEN 256
char* i2a(int val, int base, char ** ps);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

#endif

