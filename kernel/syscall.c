#include "kernel/syscall.h"
#include "syscall/syscall.h"
#include "driver/vga.h"
#include "driver/clock.h"
#include "driver/keyboard.h"
#include "gui/layer.h"
#include "gui/graphic.h"
#include "lib/fifo.h"
#include "kernel/task.h"
#include "kernel/memory.h"
#include "kernel/cpu.h"

#include "driver/mouse.h"

/**
	添加一个系统调用步骤：
	1.sys_call_table中添加函数名
	2.系统调用函数体和头文件
	3.在user的syscall中添加中断调用和头文件
*/

sys_call_t sys_call_table[NR_SYS_CALL];

struct syscall_info syscall_info;

extern struct layer *active_layer;
extern struct layer *layer_console;

void init_syscall()
{
	sys_call_table[SYS_CALL_FUNC+0] = sys_getTicks;
	sys_call_table[SYS_CALL_FUNC+1] = sys_print;
	sys_call_table[SYS_CALL_FUNC+17] = sys_creatWindow;
	sys_call_table[SYS_CALL_FUNC+18] = sys_drawSquare;
	sys_call_table[SYS_CALL_FUNC+19] = sys_fillColor;
	sys_call_table[SYS_CALL_FUNC+20] = sys_drawString;
	sys_call_table[SYS_CALL_FUNC+21] = sys_drawPoint;
	sys_call_table[SYS_CALL_FUNC+22] = sys_refreshWindow;
	sys_call_table[SYS_CALL_FUNC+23] = sys_drawLine;
	sys_call_table[SYS_CALL_FUNC+24] = sys_closeWindow;
	sys_call_table[SYS_CALL_FUNC+25] = sys_getChar;
	sys_call_table[SYS_CALL_FUNC+26] = sys_closeTask;
	sys_call_table[SYS_CALL_FUNC+27] = sys_drawHex;
	sys_call_table[SYS_CALL_FUNC+28] = sys_milliDelay;
	sys_call_table[SYS_CALL_FUNC+29] = sys_fontColor;
	sys_call_table[SYS_CALL_FUNC+30] = sys_drawInt;
	sys_call_table[SYS_CALL_FUNC+31] = sys_malloc;
	sys_call_table[SYS_CALL_FUNC+32] = sys_mfree;
	sys_call_table[SYS_CALL_FUNC+33] = sys_cpuRdtsc;
	sys_call_table[SYS_CALL_FUNC+34] = sys_cpuCpuid;
	sys_call_table[SYS_CALL_FUNC+35] = sys_readSector;
	sys_call_table[SYS_CALL_FUNC+36] = sys_writeSector;
}

/*************************************
*系统调用函数
**************************************
*/
void sys_readSector(int lba, void *buf, int counts)
{
	hd_read(0, lba, buf, counts);
}
void sys_writeSector(int lba, void *buf, int counts)
{
	hd_write(0, lba, buf, counts);
}


void sys_cpuRdtsc(int *high, int *low)
{
	asm_rdtsc(high, low);
}

void sys_cpuCpuid(char *name, int *family, int *model,int *stepping)
{
	strcpy(name, cpu.name_string);
	*family = cpu.family;
	*model = cpu.model;
	*stepping = cpu.stepping;
}

int sys_getTicks()
{
	return ticks;
}

int sys_print(char* buf, int len)
{
	char* p = buf;
	int i = len;

	while (i) {
		put_char( *p++);
		i--;
	}
	return 0;
}


//需要返回对象
int sys_creatWindow(short *buf, int wide, int high, int color_invisible, char *title)
{
	struct task *task = task_current();
	
	struct layer *layer;
	layer = layer_alloc(LAYER_TYPE_WINDOW);
	layer_init(layer, buf, wide, high, color_invisible);
	window_draw(layer->buf,wide, high, title, 1);
	layer_shift(layer, mouse.x - wide/2,  mouse.y-4);
	layer_height(layer, layer_manage->top);
	task->layer = layer;
	
	layer->task = task;
	return (int )layer;
}

//需要指定对象的
void sys_drawSquare(int window, int x, int y, int wide, int high)
{
	struct layer *layer = (struct layer *)window;
	draw_square16(layer->buf,layer->wide, x, y, wide,high, syscall_info.fillColor);
	layer_refresh(layer, x, y, x + wide + 1, y + high + 1);
}

void sys_fillColor(int16_t color)
{
	syscall_info.fillColor = color;
}

void sys_fontColor(int16_t color)
{
	syscall_info.fontColor = color;
}

//需要指定对象的
void sys_drawString(int window, int x, int y, char *string)
{
	struct layer *layer = (struct layer *)window;
	char *str = string;
	int str_len = strlen(str);
	draw_string16(layer->buf,layer->wide, x, y, syscall_info.fontColor, string);
	layer_refresh(layer, x, y, x + str_len*8+1, y + 16+1);
}

//需要指定对象的
void sys_drawPoint(int window, int x, int y)
{
	struct layer *layer = (struct layer *)window;
	draw_square16(layer->buf,layer->wide, x, y, 1,1, syscall_info.fillColor);
	layer_refresh(layer, x, y, x + 1, y + 1);
}

//需要指定对象的
void sys_refreshWindow(int window, int x, int y, int x1, int y1)
{
	struct layer *layer = (struct layer *)window;
	layer_refresh(layer, x, y, x1, y1);
}


//需要指定对象的
void sys_drawLine(int window, int x, int y, int x1, int y1)
{
	struct layer *layer = (struct layer *)window;
	draw_line16(layer->buf, layer->wide, x, y, x1, y1, syscall_info.fillColor);
	layer_refresh(layer, x, y, x1+1, y1+1);
}

//需要指定对象的
void sys_closeWindow(int window)
{
	struct layer *layer = (struct layer *)window;
	if(active_layer == layer){
		active_layer = layer_console;
	}
	layer_free(layer);
}


//需要指定对象的
int sys_getChar()
{
	struct task *task = task_current();
	int ch = -1;
	for(;;){
		if(task->key_data == -1){
			return ch;
		}
		ch = task->key_data;
		task->key_data = -1;
		return ch;
	}
}

//需要指定对象的
void sys_closeTask()
{
	int i, j;
	struct task *task = task_current();
	console_tip("Close window -KEYBOARD");
	io_cli();
	//寻找当前任务
	for(i = 0; i < task_running; i++){
		if(tasks_ptr[i] == task)break;
	}
	//把它删除
	/*实体的数据修改*/
	tasks_table[tasks_ptr[i]->pid].status = TASK_UNUSED;
	tasks_table[tasks_ptr[i]->pid].pid = 0;
	/*运行中的部分内容修改*/
	tasks_ptr[i]->status = TASK_UNUSED;
	tasks_ptr[i]->pid = 0;
	
	kernel_free_page(tasks_ptr[i]->esp_addr-PAGE_SIZE*task->stack_page_count, task->stack_page_count);
	if(i == 0){
		for(j = 1; j < task_running; j++) tasks_ptr[j-1] = tasks_ptr[j];
	}else if(i == task_running -1){
		tasks_ptr[i] = NULL;
	}else{
		for(; i < task_running; i++){
			tasks_ptr[i] = tasks_ptr[i + 1];
		}
	}
	task_running--;
	task_now--;
	//切换任务
	task_switch();
	task_ready = task_current();
	if(task_ready->status == TASK_READY){
		task_ready->status = TASK_RUNNING;
		task_ready->ticks = task_ready->priority;
		//更新cr3准备切换任务
		update_tss_cr3(task_ready);
		switch_to();
	}
	io_sti();
}

//需要指定对象的
void sys_drawHex(int window, int x, int y, int hex)
{
	struct layer *layer = (struct layer *)window;
	draw_hex16(layer->buf,layer->wide, x, y, syscall_info.fontColor, hex);
	layer_refresh(layer, x, y, x + 10*8+1, y + 16+1);
}

//需要指定对象的
void sys_drawInt(int window, int x, int y, int num)
{
	struct layer *layer = (struct layer *)window;
	draw_int16(layer->buf,layer->wide, x, y, syscall_info.fontColor, num);
	layer_refresh(layer, x, y, x + 10*8+1, y + 16+1);
}

void sys_milliDelay(int milliSec)
{
    int t = sys_getTicks();
	io_sti();
    while(((sys_getTicks() - t) * 1000/HZ) < milliSec) {}
}
