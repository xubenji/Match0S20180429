#include "command/command.h"

void builtin_ls(char *cmd)
{
	
	struct block *dir_e;
	char *name = cmd;
	name+=3;
	if(name[0] == 0)name[0] = '/';	//一定要是空格
	//console_string(name);
	if(!strcmp(name,"/") || !strcmp(name," ")){
		name[0] = '/';
	}
	struct block *dir = sys_opendir(name);
	sys_rewinddir(dir);
	console_string("\n");
	if (dir) {
		char *type = NULL;
		dir_e = NULL;
		while ((dir_e = sys_readdir(dir))) {
			console_string(" ");
			console_string(dir_e->name);
		}
		console_string("\n");
	} else {
		console_string(name);
		console_string(" open falied!\n");
	}
	//console_string(name);
	dir_close(dir);
	
	//console_string("\n");
}
