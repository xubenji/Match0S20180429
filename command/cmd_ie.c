#include "command/command.h"

void builtin_ie()
{
	uint8_t *buf = sys_malloc(SECTOR_SIZE*11);
	int *p;
	hd_read(0, 260, buf, 10);
	p = (int *)buf;
	int i;
	int va = 0;
	/*draw_hex16(layer_desktop->buf,layer_desktop->wide, 100, 100-16, COLOR_WHITE, p[0]);
	layer_refresh(layer_desktop, 0, 0, 200, 200);
	*/
	struct task *task;
	struct task *t = task_current();
	task = task_alloc(TASK_TYPE_USER);
	task->stack_page_count = 10;
	task->esp_addr = ((uint32_t )kernel_alloc_page(task->stack_page_count) + PAGE_SIZE*task->stack_page_count);
	
	//现在内核中添加内存状态
	
	for(i = 0; i < 10; i++){
		fill_vir_page_talbe(va);
		va += 0x1000;
	}
	memcpy(0, buf, 512*10);
	
	//复制给任务
	memcpy(task->ptr_pdir, PAGE_DIR_VIR_ADDR, 2048);
	task->cr3 = addr_v2p((int )task->ptr_pdir);
	
	//初始化为eip = 0即可运行
	task_init(task, (int )0x00, task->esp_addr,"APP");
	task_run(task, 1);
	
	console_string("\nRun a app from hard disk!\n");
}
