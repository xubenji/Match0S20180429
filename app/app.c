#include "syscall/syscall.h"
#include "gui/graphic.h"

#define BLOCKS_MAX 20
#define BLOCK_FREE -1
#define BLOCK_USED 0

#define TYPE_UNKNOWN -1
#define TYPE_TOKEN 0
#define TYPE_TEXT 1

#define TK_UNKNOWN -1
#define TK_HTML  0
#define TK_TITLE 1
#define TK_P         2
#define TK_HEAD        3
#define TK_BODY        4
#define TK_BR        5
#define TK_DIV        6

#define YESNO 0
#define DEBUG YESNO

// 块的结构
struct block {
  int type;                     // 块的类型
  int flags;                    // 块的使用状态
  int deep;                     // 块的深度
  char name[12];                /* 块的名字
                                   如果块的类型是token那么name表示符号名字，如果是text，那么名字是text */
  char *text;                   /* 块的文本
                                   如果块的类型是token（符号），那么text指向的是下一个块的地址，如果是text，那么指向的是要显示的文本的内容 
                                 */
};
// 声明块的数组
struct block block_table[BLOCKS_MAX];
// 指向文件的指针

int deep;                       // 记录deep的全局变量
int tk_type;                    // 记录符号类型
char *skip_token(char *buf);
struct block *pharse_token(char *buf);

char *pharse_text(char *buf);
void init();
void block_list();
struct block *alloc_block();
struct block *get_block(int deep);
void free_block(int deep);
void deep_updown(int updown);
// 处理块的类型
void pharse_type(struct block *block);
// 执行块的内容
char *run_block(struct block *block);
struct block *pharse_block(char *ptr);
uint16_t *window_buf;
int window;
int line;

void main(void)
{
	//char *s = "hello world";
	int key;
	
	char *buf;
	char *hd_buf;
  int i;
  struct block *block;
  //读取文件
	line = 0;
	window_buf = (int16_t *)malloc(320*200*2);
	hd_buf = (char *)malloc(512*11);
	
	//打开一个窗口
	window = creatWindow(window_buf, 320, 200, 0xf0f0, "IE");
	
	readSector(300, hd_buf, 10);
	buf = hd_buf;
	
	init();
	
	 // 读取第一个块
	do {
    block = pharse_block(buf);
    // 解析块
    pharse_type(block);
    // 运行块，如果有文字就显示
    buf = run_block(block);
  } while (!(strcmp(block->name, "/html") == 0)); // 判断是否结束

  // 释放当前深度
  free_block(deep);
  deep_updown(-1);
  // 释放最低深度
  free_block(deep);

	for(;;){
		milliDelay(100);
		key = getChar();
		if(key != -1 ){
			if(key == 'q')break;
		}
	}
	mfree(window_buf);
	closeWindow(window);
	closeTask();
}

struct block *pharse_block(char *ptr) {
  struct block *block;
  // 读取第一个块，必然是html
  block = pharse_token(ptr);
  deep_updown(1);               // 深度增加

  return block;
}


char *run_block(struct block *block) {
  char *p;
  struct block *b = block;
  p = b->text;
  // 有输出的另外处理
  if (TK_P == tk_type || TK_TITLE == tk_type) {
    // 读取文本内容，deep最高
    p = pharse_text(b->text);
    // deep -1是上一个块的深度，就是文本所属的块的深度

    deep_updown(-1);
    b = pharse_token(p);
    deep_updown(1);
    p = b->text;

    b = get_block(deep);
    switch (tk_type) {
		case TK_TITLE:
			drawString(window, 4+16+4, 4, b->text);
			break;
		case TK_P:
			drawString(window, 4, 24 + line*16, b->text);
			line++;
        break;
		default: break;
    }

    // 文本块的深度
    free_block(deep);
    // 文本类型的深度
    deep_updown(-1);
    free_block(deep);

    // 文本类型的深度
    // deep_updown(-1);

    // 上一级块的深度

  } else {
    free_block(deep);
    // 文本类型的深度
    deep_updown(-1);
  }


  return p;
}

void pharse_type(struct block *block) {
  if (strcmp(block->name, "html") == 0) {
    tk_type = TK_HTML;
  } else if (strcmp(block->name, "head") == 0) {
    tk_type = TK_HEAD;
  } else if (strcmp(block->name, "body") == 0) {
    tk_type = TK_BODY;
  } else if (strcmp(block->name, "br") == 0) {
    tk_type = TK_BR;
  } else if (strcmp(block->name, "div") == 0) {
    tk_type = TK_DIV;

  } else if (strcmp(block->name, "p") == 0) {
    tk_type = TK_P;
  } else if (strcmp(block->name, "title") == 0) {
    tk_type = TK_TITLE;

  } else {
    tk_type = TK_UNKNOWN;

  }
}
void deep_updown(int updown) {
  deep += updown;
}

// 打印已经分配的块
void block_list() {
  int i;
 // printf("\nlist\n");
  for (i = 0; i < BLOCKS_MAX; i++) {
    if (block_table[i].flags != BLOCK_FREE) {
     // printf("block:%d name:%s\n", i, block_table[i].name);
     // printf("deep:%d text:%s\n", block_table[i].deep, block_table[i].text);
    }
  }
}
// 初始化
void init() {
  deep = 0;
  int i;
  for (i = 0; i < BLOCKS_MAX; i++) {
    block_table[i].flags = BLOCK_FREE;
    block_table[i].type = TYPE_UNKNOWN;
  }
}
// 跳过空白和回车
char *skip_token(char *buf) {
  while (*buf == ' ' || *buf == '\n')
    buf++;
  return buf;
}
/* 
   解析符号 解析出一个块 内容： 块名字（p html
   head...）和下一个块（p ptr）的指针 */
struct block *pharse_token(char *buf) {
  char *p = buf;
  int i;
  p = skip_token(p);
  // char end_name[12];
  if (*p++ == '<') {
    struct block *b = (struct block *)alloc_block();
    b->type = TYPE_TOKEN;
    b->deep = deep;
    p = skip_token(p);

    for (i = 0; *p != ' ' && *p != '>'; i++) {
      b->name[i] = *p++;
    }
    if (DEBUG) {
     // printf("b name%s\n", b->name);
    }
    p = skip_token(p);
    if (*p++ != '>') {
      //printf("Error: no '>'\n");
    }
    b->text = p;
    return b;
  } else {
   // printf("Error: no '<'\n");
  }
}
/* 
   解析文本 内容： 块名字（text）和文本（text ptr）的指针

 */
char *pharse_text(char *buf) {
  char *p = buf;
  int i;
  struct block *b = (struct block *)alloc_block();
  b->type = TYPE_TEXT;
  b->text = (char *)malloc(256);
  b->deep = deep;
  p = skip_token(p);

  strcpy(b->name, "text");
  for (i = 0; *p != '<'; i++) {
    b->text[i] = *p++;
  }
  if (DEBUG) {
    //printf("%s\n", b->text);
  }
  p = skip_token(p);

  return p;
}
// 分配一个块
struct block *alloc_block() {
  int i;
  for (i = 0; i < BLOCKS_MAX; i++) {
    if (block_table[i].flags == BLOCK_FREE) {
      block_table[i].flags = BLOCK_USED;
      memset(block_table[i].name, 0, 12);
      block_table[i].deep = -1;
      if (DEBUG) {
        //printf("Alloc block:%d\n", i);
      }
      return &block_table[i];
    }
  }
  //printf("!!Alloc error");
}
// 通过deep获取一个块
struct block *get_block(int deep) {
  int i;
  for (i = 0; i < BLOCKS_MAX; i++) {
    if (block_table[i].flags != BLOCK_FREE) {

      if (block_table[i].deep == deep) {
        if (DEBUG) {
         // printf("Get block:%d\n", i);
        }
        return &block_table[i];
      }
    }
  }
  //printf("!!Alloc error\n");
}
// 释放deep一样的所有块
void free_block(int deep) {
  int i;
  for (i = 0; i < BLOCKS_MAX; i++) {
    if (block_table[i].flags != BLOCK_FREE) {

      if (block_table[i].deep == deep) {
        if (DEBUG) {
          //printf("Free block:%d\n", i);
        }
        block_table[i].flags = BLOCK_FREE;
        memset(block_table[i].name, 0, 12);
        block_table[i].deep = -1;
        if (block_table[i].type == TYPE_TEXT) {
          mfree(block_table[i].text);
        }
        block_table[i].type = TYPE_UNKNOWN;
      }
    }
  }

}
