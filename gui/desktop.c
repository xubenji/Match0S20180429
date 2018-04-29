#include "gui/gui.h"
#include "gui/layer.h"
#include "gui/graphic.h"
#include "kernel/memory.h"
#include "driver/vga.h"
#include "driver/video.h"
#include "driver/console.h"
#include "kernel/ards.h"
#include "kernel/task.h"
#include "gui/desktop.h"

struct task_bar_block tbb_table[TBB_NR];

struct task_bar_block *tbb_desktop, *tbb_console;
extern struct layer *layer_taskbar_down,*layer_desktop;

struct FIFO32 desktop_fifo;

int desktop_buf[64];

void init_desktop()
{
	int i;
	fifo32_init(&desktop_fifo, 64, desktop_buf);
	for(i = 0; i < TBB_NR; i++){
		tbb_table[i].status = TBB_STATUS_FREE;
	}
}

struct task_bar_block *alloc_tbb()
{
	int i;
	for(i = 0; i < TBB_NR; i++){
		if(tbb_table[i].status == TBB_STATUS_FREE){
			tbb_table[i].status = TBB_STATUS_OFF;
			return &tbb_table[i];
		}
	}
	return NULL;
}

void tbb_display()
{
	int i;
	for(i = 0; i < TBB_NR; i++){
		if(tbb_table[i].status != TBB_STATUS_FREE){
			tbb_show(&tbb_table[i]);
		}
	}
}

void task_desktop_entry()
{
	int i = 0;
	struct task *task = task_current();
	
	
	
	//tbb_console = alloc_tbb();
	//tbb_init(tbb_console,1, TBB_STATUS_OFF);
	/*draw_square16(layer_desktop->buf,layer_desktop->wide,0,100,100,100, 0);
	
	layer_refresh(layer_desktop, 0,100, 100, 200);
	*/
	
	
	
	
	//printf("Driver of keyboard is running.\n");
	for(;;){
		if(fifo32_status(&desktop_fifo) == 0){
			sys_milliDelay(100);
			fifo32_put(&desktop_fifo, 1);
			task_deprive(task);
		} else {
			if(fifo32_status(&desktop_fifo) != 0){
				//sys_milliDelay(10000);
				i = fifo32_get(&desktop_fifo);
				if(i == 1){
					tbb_display();
				}
				
				
			}
		}
		
	}
	
}

void tbb_init(struct task_bar_block *tbb, int pos, int status)
{
	tbb->x = pos;
	tbb->y = 2;
	tbb->wide = TBB_BLOCK;
	tbb->high = TBB_BLOCK;
	tbb->color = COLOR_WHITE;
	tbb->status = status;
}

void tbb_show(struct task_bar_block *tbb)
{
	draw_square16(layer_desktop->buf,layer_desktop->wide,4+tbb->x*TBB_SIZE,layer_desktop->high-TASKBAR_DOWN_HIGH+tbb->y,tbb->wide,tbb->high, COLOR_WHITE150);
	
	if(tbb->status == TBB_STATUS_ON){
		tbb->color = COLOR_WHITE230;
	}else{
		tbb->color = COLOR_WHITE200;
	}
	
	draw_square16(layer_desktop->buf,layer_desktop->wide,4+tbb->x*TBB_SIZE,layer_desktop->high-TASKBAR_DOWN_HIGH+tbb->y,tbb->wide,tbb->high, tbb->color);
	layer_refresh(layer_desktop, tbb->x,tbb->y, 4+tbb->x*TBB_SIZE + tbb->wide, layer_desktop->high-TASKBAR_DOWN_HIGH+tbb->y + tbb->high);
}


