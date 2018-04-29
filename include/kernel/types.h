#ifndef INCLUDE_TYPES_H_
#define INCLUDE_TYPES_H_

#define NULL ((void*)0)
#define bool int
#define true 1
#define false 0
#define DIV_ROUND_UP(X, STEP) ((X + STEP - 1) / (STEP))

typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;

typedef void (*irq_handler_t) (int irq);
typedef void* sys_call_t;
typedef	char *	va_list;
typedef void* application_t;

#endif  // INCLUDE_TYPES_H_