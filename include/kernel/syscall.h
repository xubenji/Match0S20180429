#ifndef _SYSCALL_H
#define _SYSCALL_H
#include "kernel/types.h"
#include "gui/layer.h"
#include "lib/fifo.h"

#define NR_SYS_CALL 100
#define SYS_CALL_INFO 0			//系统调用信息的位置
#define SYS_CALL_FUNC (SYS_CALL_INFO+NR_SYS_CALL/2)			//系统调用函数的位置

extern sys_call_t sys_call_table[NR_SYS_CALL];

#define TITLE_LEN 32
#define STRING_LEN 128
#define SYSCALL_FIFO_LEN 128

struct syscall_info
{
	int16_t fillColor, fontColor;
};
extern struct syscall_info syscall_info;

void init_syscall();

/*interrupt.asm*/
void intrrupt_sys_call();

/*syscall.c*/
/*系统调用函数*/
int sys_getTicks();

int sys_print();

int sys_creatWindow(short *buf, int wide, int high, int color_invisible, char *title);
void sys_drawSquare(int window, int x, int y, int wide, int high);
void sys_fillColor(int16_t color);
void sys_fontColor(int16_t color);
void sys_drawString(int window, int x, int y, char *string);
void sys_drawPoint(int window, int x, int y);
void sys_refreshWindow(int window, int x, int y, int x1, int y1);
void sys_drawLine(int window, int x, int y, int x1, int y1);
void sys_closeWindow(int window);
int sys_getChar();
void sys_closeTask();
void sys_drawHex(int window, int x, int y, int hex);
void sys_milliDelay(int milliSec);
void sys_drawInt(int window, int x, int y, int num);
void sys_cpuRdtsc(int *high, int *low);
void sys_cpuCpuid(char *name, int *family, int *model,int *stepping);
void sys_readSector(int lba, void *buf, int counts);
void sys_writeSector(int lba, void *buf, int counts);

#endif

