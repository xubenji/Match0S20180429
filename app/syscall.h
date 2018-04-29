#ifndef _USER_SYSCALL_H
#define _USER_SYSCALL_H
#include "kernel/types.h"
#include "kernel/syscall.h"
/*函数调用*/
int printf(char* buf, int len);

int getTicks();
/**返回图层地址 （int）*/
int creatWindow(short *buf, int wide, int high, int color_invisible, char *title);
/**绘制一个矩形*/
void drawSquare(int window, int x, int y, int x1, int y1);
/**绘制一个整数*/
void sys_drawInt(int window, int x, int y, int num);
/**设置图形颜色*/
void fillColor(int16_t color);
void fontColor(int16_t color);
/**绘制一个字符串*/
void drawString(int window, int x, int y, char *string);
/**绘制一个点*/
void drawPoint(int window, int x, int y);
/**刷新窗口的指定位置*/
void refreshWindow(int window, int x, int y, int x1, int y1);
/**绘制一条直线*/
void drawLine(int window, int x, int y, int x1, int y1);
/**绘制一个窗口*/
void closeWindow(int window);
/**从键盘获取一个字符*/
int getChar();
/**关闭任务*/
void closeTask();
/**绘制16进制数*/
void drawHex(int window, int x, int y, int hex);
/**延迟函数*/
void milliDelay(int milliSec);
/**内存的分配和释放*/
void* malloc(uint32_t size);
void mfree(void* ptr);
void cpuRdtsc(int *high, int *low);
void cpuCpuid(char *name, int *family, int *model,int *stepping);
#endif  // INCLUDE_TYPES_H_