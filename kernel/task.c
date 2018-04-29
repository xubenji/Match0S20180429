#include "kernel/types.h"
#include "driver/vga.h"
#include "kernel/memory.h"
#include "print/printk.h"
#include "kernel/task.h"
#include "kernel/descriptor.h"
#include "kernel/system.h"
#include "driver/mouse.h"
#include "driver/keyboard.h"
#include "driver/console.h"
#include "driver/clock.h"
#include "driver/cmos.h"
#include "driver/hd.h"
#include "gui/layer.h"
#include "gui/desktop.h"

struct task tasks_table[MAX_TASKS] = {0xf,};
struct task *tasks_ptr[MAX_TASKS];
int task_now;
int task_running;

struct task *task_mouse, *task_keyboard, *task_console;
struct task *task_ready, *task_idle; //添加一个闲置任务，用于任务结束后跳转到此
struct task *task_system, *task_hd;
struct task *task_desktop;

extern struct layer *layer_desktop, *layer_mouse, *layer_console, *layer_taskbar;
void init_task()
{
	int i;
	
	init_tss();
	
	for(i = 0; i < MAX_TASKS; i++){
		tasks_table[i].status = TASK_UNUSED;
		tasks_table[i].pid = 0;	//释放任务时要设置pid
	}
	task_running = 0;
	task_now = 0;
	
	
	task_mouse = task_alloc(TASK_TYPE_DRIVER);
	task_init(task_mouse, (int )&task_mouse_entry, ((int )sys_malloc(2) + PAGE_SIZE*2), "mouse");
	task_run(task_mouse, 5);
	
	task_keyboard = task_alloc(TASK_TYPE_DRIVER);
	task_init(task_keyboard, (int )&task_keyboard_entry, ((int )kernel_alloc_page(2) + PAGE_SIZE*2), "keyboard");
	task_run(task_keyboard, 2);
	
	task_console = task_alloc(TASK_TYPE_DRIVER);
	task_init(task_console, (int )&task_console_entry, ((int )kernel_alloc_page(2) + PAGE_SIZE*2), "console");
	task_run(task_console, 2);
	task_console->tbb = alloc_tbb();
	tbb_init(task_console->tbb,0, TBB_STATUS_OFF);
	
	task_system = task_alloc(TASK_TYPE_DRIVER);
	task_init(task_system, (int )&task_system_entry, ((int )kernel_alloc_page(1) + PAGE_SIZE*1), "system");
	task_run(task_system, 1);
	
	task_desktop = task_alloc(TASK_TYPE_DRIVER);
	task_init(task_desktop, (int )&task_desktop_entry, ((int )kernel_alloc_page(2) + PAGE_SIZE*2), "desktop");
	task_run(task_desktop, 3);
	task_desktop->tbb = alloc_tbb();
	tbb_init(task_desktop->tbb,1, TBB_STATUS_ON);
	
	task_idle = task_alloc(TASK_TYPE_USER);
	task_init(task_idle, (int )&task_idle_entry, ((int )kernel_alloc_page(1) + PAGE_SIZE), "dile");
	//task_run(task_idle, 1);
	
	task_hd = task_alloc(TASK_TYPE_DRIVER);
	task_init(task_hd, (int )&task_hd_entry, ((int )kernel_alloc_page(1) + PAGE_SIZE), "hd");
	task_run(task_hd, 1000);
	
	
	
	
	layer_console->task = task_console;
	task_console->layer = layer_console;
	
	task_ready = task_mouse;
	update_tss_cr3(task_ready);
	printk(">init task\n");
}

struct task *task_alloc(char type)
{
	int i;
	struct task *task;
	int *p;

	for(i = 0; i < MAX_TASKS; i++){
		if(tasks_table[i].status == TASK_UNUSED){
			task = &tasks_table[i];
			task->status = TASK_READY;
			task->type = type;
			if(task->type == TASK_TYPE_DRIVER){
				task->regs.cs = DRIVER_CODE_SEL;
				task->regs.ds = DRIVER_DATA_SEL;
				task->regs.es = DRIVER_DATA_SEL;
				task->regs.fs = DRIVER_DATA_SEL;
				task->regs.gs = DRIVER_DATA_SEL;
				task->regs.ss = DRIVER_DATA_SEL;
				task->regs.eflags = EFLAGS_IOPL_1 | EFLAGS_MBS | EFLAGS_IF_1;	//支持IO
				task->cr3 = PAGE_DIR_PHY_ADDR;	//驱动就直接使用内核的页目录表
				
			}else if(task->type == TASK_TYPE_USER){
				task->regs.cs = USER_CODE_SEL;
				task->regs.ds = USER_DATA_SEL;
				task->regs.es = USER_DATA_SEL;
				task->regs.fs = USER_DATA_SEL;
				task->regs.gs = USER_DATA_SEL;
				task->regs.ss = USER_DATA_SEL;
				task->regs.eflags = EFLAGS_IOPL_0 | EFLAGS_MBS | EFLAGS_IF_1;	//不支持IO
				//用户就创建新的
				
				
				task->ptr_pdir = (int *)copy_kernel_page();
				task->cr3 = addr_v2p((int )task->ptr_pdir);
				//填写页目录表的最后一项为现在的物理地址+属性
				p[1023] = task->cr3|0x07;
				
			}
			task->regs.eax = 0;
			task->regs.ebx = 0;
			task->regs.edx = 0;
			task->regs.ecx = 0;
			task->regs.ebp = 0;
			task->regs.esp = 0;
			task->regs.esi = 0;
			task->regs.edi = 0;			
			task->regs.eip = 0;
			task->layer = NULL;
			task->key_data = -1;
			task->deprived = 0;
			if(task->pid == 0){
				task->pid = i;
			}
			return task;
		}
	}

}

void task_run(struct task *task, int priority)
{
	task->status = TASK_RUNNING;
	task->priority = priority;
	task->ticks = task->priority;
	tasks_ptr[task_running] = task;	//将任务添加到最后
	task_running++;
}

void task_switch()
{
	if(task_running >= 2){
		task_now++;
		if(task_now == task_running){
			task_now = 0;
		}
	}
}

void task_close(struct task *task)
{
	int i, j;
	struct task *self_task = task;
	io_cli();
	//寻找当前任务
	for(i = 0; i < task_running; i++){
		if(tasks_ptr[i] == self_task)break;
	}
	//把它删除
	/*实体的数据修改*/
	tasks_table[tasks_ptr[i]->pid].status = TASK_UNUSED;
	tasks_table[tasks_ptr[i]->pid].pid = 0;
	/*运行中的部分内容修改*/
	tasks_ptr[i]->status = TASK_UNUSED;
	tasks_ptr[i]->pid = 0;
	
	kernel_free_page(tasks_ptr[i]->esp_addr-PAGE_SIZE*self_task->stack_page_count, self_task->stack_page_count);
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


struct task *task_current()
{
	return tasks_ptr[task_now];
}
//强行剥夺任务的时间片,以让任务切换为下一个任务
void task_deprive(struct task *task)
{
	if(task->deprived == 0){
		task->ticks = 1;
		task->deprived = 1;
	}
}

void task_idle_entry()
{
	int i;
	struct task *tast = task_current();
	for(;;){
		
		
		//io_hlt();	//减缓运行
	}
}

void task_name(struct task *task, char *name)
{
	char *str = name;
	int i;
	/**一共24个字符，不足的用空白填充,便于打印*/
	for(i = 0; i < TASK_NAME_LEN; i++){
		if(*str != 0){
			task->name[i] = *str;
			str++;
		}else{
			task->name[i] = ' ';
		}
	}
	task->name[TASK_NAME_LEN] = 0;
}

void task_init(struct task *task, int entry, int stack, char *name)
{
	task->regs.eip = entry;
	task->regs.esp = stack;
	task_name(task, name);
}
