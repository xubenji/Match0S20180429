#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "kernel/types.h"
#include "driver/console.h"
#include "kernel/memory.h"

#define NR_APPLICATIONS 10
#define APPLICATIONS_FUNC 0
#define APPLICATION_NAME_LEN 12


/**定义结构体*/
#define WINDOW int


struct application 
{
	char name[APPLICATION_NAME_LEN];
	application_t func;
};
extern struct application application_table[];

void init_application();
int application_command(char *command);
void application_init(int idx, char *name, application_t func);
//app function
void app_walk();
void app_2048();
void app_test();
void app_cpuid();





#endif

