#include "command/command.h"

void builtin_ptask()
{
	int i;
	/*24 12 6*/
	console_string("\nTASK                    TYPE        PID   \n");
	for(i = 0; i < task_running; i++){
		if(tasks_ptr[i]->status != TASK_UNUSED){
			console_string(tasks_ptr[i]->name);	
			
			if(tasks_ptr[i]->type == TASK_TYPE_USER){
				console_string("USER        ");
			}else if(tasks_ptr[i]->type == TASK_TYPE_DRIVER) {
				console_string("DRIVER      ");
			}else {
				console_string("Unknown     ");
			}

			console_int(tasks_ptr[i]->pid);	
			console_string("\n");
			
			
			
		}
	}
}