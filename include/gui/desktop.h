#ifndef _DESKTOP_H
#define	_DESKTOP_H
#include "kernel/types.h"

#define TBB_NR 10


#define TBB_STATUS_ON 2
#define TBB_STATUS_OFF 1
#define TBB_STATUS_FREE 0

#define TASKBAR_UP_HIGH 20
#define TASKBAR_DOWN_HIGH 36

#define TBB_SIZE 36
#define TBB_BLOCK 32



//task bar block struct
struct task_bar_block
{
	int x, y;		//位置
	int wide, high;	//宽高
	int status;		//状态
	int color;
};


void task_desktop_entry();

struct task_bar_block *alloc_tbb();
void tbb_init(struct task_bar_block *tbb, int pos, int status);
void tbb_show(struct task_bar_block *tbb);
void tbb_display();
#endif
