#include "application/application.h"
#include "driver/console.h"
#include "kernel/memory.h"

struct application application_table[NR_APPLICATIONS];

void init_application()
{
	application_init(APPLICATIONS_FUNC + 0, "walk", app_walk);
	application_init(APPLICATIONS_FUNC + 1, "2048", app_2048);
	application_init(APPLICATIONS_FUNC + 2, "test", app_test);
	application_init(APPLICATIONS_FUNC + 3, "cpuid", app_cpuid);
	
}

void application_init(int idx, char *name, application_t func)
{
	strcpy(application_table[idx].name, name);
	application_table[idx].func = func;
}

int application_command(char *command)
{
	struct task *task;
	char name[APPLICATION_NAME_LEN];
	int i, j;
	
	for(i = 0; i < APPLICATION_NAME_LEN; i++){
		if(command[i] <= ' '){
			break;
		}
		name[i] = command[i];	//获取命令内容
	}
	name[i] = 0;
	/*
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 200, COLOR_RED,application_table[0].name);
	layer_refresh(layer_desktop, 0, 200, 100, 200+16);
	*/
	//寻找应用程序
	for(j = 0; j < NR_APPLICATIONS; j++){
		if(strcmp(name, application_table[j].name) == 0){
			task = task_alloc(TASK_TYPE_USER);
			task->stack_page_count = 1;
			task->esp_addr = ((uint32_t )kernel_alloc_page(task->stack_page_count) + PAGE_SIZE*task->stack_page_count);
			
			/*task->regs.eip = (int )application_table[j].func;
			task->regs.esp = task->esp_addr = ((uint32_t )kernel_alloc_page(task->stack_page_count) + PAGE_SIZE*task->stack_page_count);
			*/
			task_init(task, (int )application_table[j].func, task->esp_addr,application_table[j].name);
			
			task_run(task, 1);
			console_newline();
			return 1;
		}
	}
	return 0;
}
