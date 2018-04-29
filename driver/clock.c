#include "kernel/task.h"
#include "kernel/io.h"
#include "driver/clock.h"
#include "kernel/descriptor.h"
#include "kernel/task.h"
#include "kernel/tss.h"

int ticks;

void init_clock(void)
{
	//io_out8(PIT_CTRL, 0x34);
	/*io_out8(PIT_CNT0, (unsigned char) (TIMER_FREQ/HZ));
	io_out8(PIT_CNT0, (unsigned char) ((TIMER_FREQ/HZ) >> 8));
	*/
	//0.01秒触发一次中断
	io_out8(PIT_CNT0, (unsigned char) (TIMER_FREQ/HZ));
	io_out8(PIT_CNT0, (unsigned char) ((TIMER_FREQ/HZ) >> 8));
	/*io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, 0x9c);
	io_out8(PIT_CNT0, 0x2e);
	*/
	ticks = 0;
	
	put_irq_handler(CLOCK_IRQ, clock_handler);
	enable_irq(CLOCK_IRQ);
	put_str(">init clock\n");
}


void clock_handler(int irq)
{
	struct task *task = task_current();
	ticks++;
	
	task->ticks--;
	//当前任务的ticks为0才进行任务调度
	if(task->ticks <= 0){
		schdule();
	}
}

void schdule()
{
	struct task *task = task_current();
	int i;
	if(task->status == TASK_RUNNING){
		//设置优先级和状态
		task->ticks = task->priority;
		task->status = TASK_READY;
		//切换任务
			task_switch();
			//设置新任务状态
			task_ready = task_current();
			
			//如果优先级没有就说明是一个执行后有结束的任务
			//if(task_ready->status == TASK_READY){
				task_ready->status = TASK_RUNNING;
				task_ready->ticks = task_ready->priority;
				task->deprived = 0;
				//更新cr3准备切换任务
				update_tss_cr3(task_ready);
			//}
	}
}
