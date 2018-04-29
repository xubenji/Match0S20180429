#ifndef _LAYER_H
#define	_LAYER_H
#include "kernel/types.h"
#include "kernel/task.h"

#define	MAX_LAYERS 12

#define	LAYER_UNUSED 0		//图层未使用
#define	LAYER_USE 1			//图层使用中


#define	LAYER_TYPE_UNKNOWN -1			//图层为不能移动类型
#define	LAYER_TYPE_NOTMOVE 0			//图层为不能移动类型
#define	LAYER_TYPE_MOVE    1			//图层为能移动类型

#define	LAYER_TYPE_WINDOW  2			//图层为窗口

#define	LAYER_INVISIBLE  0xfffe			//图层为窗口
#define	LAYER_NOINVISIBLE  0xffffffff			//图层为窗口

struct layer 
{
	uint16_t *buf;			//图层的数据区
	int wide ,high;	//图层的宽高
	int x, y;				//图层的x，y的偏移，相对于屏幕
	int32_t color_invisible;	//透明图层颜色号
	int height;				//图层的高度，在图层管理中的高度
	int status;				//图层的状态
	char title[8];
	int type;				//图层的类型
	bool active;
	struct task *task;
};

struct layer_manage 
{
	uint16_t *vram, *map;	//map用来记录图层的id信息
	int wide, high, top;	//top用来记录最高层的图层	
	struct layer layers_table[MAX_LAYERS];		//图层的数组
	struct layer *layers_ptr[MAX_LAYERS];		//图层的指针
	
};

extern struct layer_manage *layer_manage;


struct layer_manage *layer_manage_init(uint16_t *vram, int wide, int high);
struct layer *layer_alloc(int type);
void layer_init(struct layer *layer, uint16_t *buf, int wide, int high, int32_t color_invisible);
void layer_height(struct layer *layer, int height);
void layer_shift(struct layer *layer, int x, int y);
void layer_free(struct layer *layer);
void layer_refreshsub(int x0, int y0, int x1, int y1, int h0, int h1);
void layer_refresh(struct layer *layer, int x0, int y0, int x1, int y1);
void layer_refreshmap(int x0, int y0, int x1, int y1, int h0);
int layer_activity(struct layer *active_layer);
int layer_inactive(struct layer *active_layer);

void window_change(struct layer *layer, char activate);
void window_on(struct layer *layer);
void window_off(struct layer *layer);

void layer_string(struct layer *layer, uint32_t x,uint32_t y, uint8_t *s, uint16_t color);
void layer_integer(struct layer *layer, uint32_t x,uint32_t y, uint8_t hex, uint32_t integer, uint16_t color);
void layer_square(struct layer *layer, uint32_t x,uint32_t y, uint32_t wide,uint32_t high, uint16_t color);

void layer_char(struct layer *layer, uint32_t x,uint32_t y, uint8_t *s, uint16_t color);
void layer_pix(struct layer *layer, uint32_t x,uint32_t y, uint16_t color);
int32_t layer_image_bmp(struct layer *layer, uint32_t x,uint32_t y, const char *bmpname);

#endif
