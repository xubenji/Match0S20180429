/*
copyright hu,zhiheng & xu,yanjie 2016
kernel_entry	系统的入口，在这里进行初始化，然后运行多任务
*/
#include "kernel/types.h"
#include "driver/vga.h"
#include "kernel/descriptor.h"
#include "kernel/8259a.h"
#include "driver/clock.h"
#include "driver/keyboard.h"
#include "kernel/memory.h"
#include "print/printk.h"
#include "kernel/syscall.h"
#include "kernel/task.h"
#include "driver/video.h"
#include "gui/graphic.h"
#include "gui/gui.h"
#include "application/application.h"
#include "driver/hd.h"
#include "kernel/cpu.h"
#include "fs/fs.h"

int kernel_entry()
{
	init_vga();
	init_vesa();
	init_memory();
	init_descriptor();
	
	
	init_clock();
	init_keyboard();
	init_mouse();

	init_gui();
	
	
	init_cpu();
	
	init_hd();

	init_application();
	init_task();
	switch_to();
}



