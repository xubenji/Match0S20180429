#ifndef _COMMAND_H
#define _COMMAND_H

#include "kernel/types.h"
#include "kernel/memory.h"
#include "gui/gui.h"
#include "gui/graphic.h"
#include "driver/console.h"
#include "driver/hd.h"
#include "fs/fs.h"


void builtin_cls();
void builtin_os();
void builtin_help();
void builtin_ptask();
void builtin_mem();
void builtin_hd();
void builtin_ie();
void builtin_ls(char *cmd);
#endif

