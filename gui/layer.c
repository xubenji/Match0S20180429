#include "gui/layer.h"
#include "gui/graphic.h"
#include "kernel/memory.h"
#include "driver/vga.h"
#include "driver/video.h"
extern struct layer *active_layer;



struct layer_manage *layer_manage_init(uint16_t *vram, int wide, int high)
{
	struct layer_manage *lym;

	int i;
	//分配空间
	lym = (struct layer_manage *) sys_malloc(sizeof(struct layer_manage));
	if(lym == 0)while(1);

	lym->map = (uint16_t *)sys_malloc(wide * high * 2);
	if(lym->map == 0){
		sys_mfree(lym);
		while(1);
	}
	
	lym->vram = vram;
	lym->wide = wide;
	lym->high = high;
	lym->top = -1;		//no layer
	
	for(i = 0; i < MAX_LAYERS; i++){
		lym->layers_table[i].status = LAYER_UNUSED;
		lym->layers_table[i].type = LAYER_TYPE_UNKNOWN;
	}
	
	return lym;
	
}

struct layer *layer_alloc(int type)
{
	struct layer *layer;
	int i;
	for(i = 0; i < MAX_LAYERS; i++){
		if(layer_manage->layers_table[i].status == LAYER_UNUSED){
			layer = &layer_manage->layers_table[i];
			layer->status = LAYER_USE;
			layer->height = -1;			//图层为隐藏状态
			layer->active = false;
			if(layer->type == LAYER_TYPE_UNKNOWN){
				layer->type = type;
			}
			return layer;
		}
	}
	return 0;	//分配失败，所有图层都在使用
}

void layer_init(struct layer *layer, uint16_t *buf, int wide, int high, int32_t color_invisible)
{
	layer->buf = buf;
	layer->wide = wide;
	layer->high = high;
	layer->color_invisible = color_invisible;
}

//改变图层高度
void layer_height(struct layer *layer, int height)
{
	struct layer_manage *lym = layer_manage;
	int h, old = layer->height;
	//修正高度
	if(height > lym->top + 1) height = lym->top + 1;
	if(height < -1) height = -1;
	
	layer->height = height;		//设定高度
	
	//再layers_ptr中重新排列
	if(old > height){	//设置的高度比以前自己高度低
		if(height >= 0){	//如果新的高度为显示（>=0）
			//把 old  和 new 之间的图层提高一层， 把old覆盖，然后new那一层添加为new图层
			//中间图层升高
			for(h = old; h > height; h--){
				lym->layers_ptr[h] = lym->layers_ptr[h - 1];	//把图层提高
				lym->layers_ptr[h]->height = h;				//设置图层高度信息				
			}
			lym->layers_ptr[height] = layer;		//写入图层
			layer_refreshmap(layer->x, layer->y, layer->x + layer->wide, layer->y + layer->high, layer->height + 1);
			layer_refreshsub(layer->x, layer->y, layer->x + layer->wide, layer->y + layer->high, layer->height + 1, old);
		}else{	/*设置成了隐藏*/
			if(lym->top > old){
				//把原来图层和最高层之间的图层往低端降低
				for(h = old; h < lym->top; h++){
					lym->layers_ptr[h] = lym->layers_ptr[h + 1];	//把图层提高
					lym->layers_ptr[h]->height = h;				//设置图层高度信息				
				}					
			}
			lym->top--;		//隐藏了一个图层，最高层--
			layer_refreshmap(layer->x, layer->y, layer->x + layer->wide, layer->y + layer->high, 0);
			layer_refreshsub(layer->x, layer->y, layer->x + layer->wide, layer->y + layer->high, 0, old - 1);
		}
		
	} else if(old < height){ //设置的高度比以前自己高度高
		if(old >= 0){	//以前处于显示中
			//把old 和new height中间的图层下降，然后填写新的相关信息
			for(h = old; h < height; h++){
				lym->layers_ptr[h] = lym->layers_ptr[h + 1];	//把图层降低
				lym->layers_ptr[h]->height = h;				//设置图层高度信息				
			}
			lym->layers_ptr[height] = layer;		//写入图层
		}else{	//原来处于隐藏状态
			//操作最高层和height之间的图层，将它们提高一层
			for(h = lym->top; h >= height; h--){
				lym->layers_ptr[h + 1] = lym->layers_ptr[h];	//把图层提高
				lym->layers_ptr[h + 1]->height = h + 1;			//设置图层高度信息				
			}
			lym->layers_ptr[height] = layer;		//写入图层
			lym->top++;		//增加显示了一个图层，最高层++
			
		}
		layer_refreshmap(layer->x, layer->y, layer->x + layer->wide, layer->y + layer->high, height);
		layer_refreshsub(layer->x, layer->y, layer->x + layer->wide, layer->y + layer->high, height, height);
	}
}	

void layer_refresh(struct layer *layer, int x0, int y0, int x1, int y1)
{
	
	if(layer->height >= 0){
		layer_refreshsub(layer->x +  x0, layer->y +  y0, layer->x +  x1, layer->y +  y1, layer->height, layer->height);
		
	}
}

void layer_refreshsub(int x0, int y0, int x1, int y1, int h0, int h1)
{
	
	int h, bx, by, x, y, bx0, by0, bx1, by1;	//x,y是图层在屏幕中的位置
	uint16_t *buf, *vram = layer_manage->vram, *map = layer_manage->map;
	uint16_t color, layer_id;
	struct layer *layer;

	if(x0 < 0){x0 = 0;}
	if(y0 < 0){y0 = 0;}
	if(x1 > layer_manage->wide){x1 = layer_manage->wide;}
	if(y1 > layer_manage->high){y1 = layer_manage->high;}
	
	for(h = h0; h <= h1; h++){
		//获取图层
		layer = layer_manage->layers_ptr[h];
		buf = layer->buf;
		layer_id = layer - layer_manage->layers_table;
		bx0 = x0 - layer->x;
		by0 = y0 - layer->y;
		bx1 = x1 - layer->x;
		by1 = y1 - layer->y;
		if(bx0 < 0){bx0 = 0;}
		if(by0 < 0){by0 = 0;}
		if(bx1 > layer->wide){bx1 = layer->wide;}
		if(by1 > layer->high){by1 = layer->high;}
		
		
		
		//循环写入数据
		for(by = by0; by < by1; by++){	//high*2才能写完全部，不然只有一半
			y = layer->y + by;
			for(bx = bx0; bx < bx1; bx++){
				x = layer->x + bx;		
				
				if(x0 <= x && x < x1 && y0 <= y && y < y1){
					//color = buf[(by * layer->wide) + bx];				
					if(map[(y * layer_manage->wide + x)] == layer_id){
						vram[(y * layer_manage->wide + x)] = buf[(by * layer->wide) + bx];		
					}
				}	
			}
		}
	}
}


void layer_refreshmap(int x0, int y0, int x1, int y1, int h0)
{
	
	int h, bx, by, x, y, bx0, by0, bx1, by1;	//x,y是图层在屏幕中的位置
	uint16_t *buf, *vram = layer_manage->vram, *map = layer_manage->map;
	int32_t color;
	struct layer *layer;
	uint16_t layer_id;
	
	if(x0 < 0){x0 = 0;}
	if(y0 < 0){y0 = 0;}
	if(x1 > layer_manage->wide){x1 = layer_manage->wide;}
	if(y1 > layer_manage->high){y1 = layer_manage->high;}
	
	for(h = h0; h <= layer_manage->top; h++){
		//获取图层
		layer = layer_manage->layers_ptr[h];
		buf = layer->buf;
		layer_id = layer - layer_manage->layers_table;
		bx0 = x0 - layer->x;
		by0 = y0 - layer->y;
		bx1 = x1 - layer->x;
		by1 = y1 - layer->y;
		if(bx0 < 0){bx0 = 0;}
		if(by0 < 0){by0 = 0;}
		if(bx1 > layer->wide){bx1 = layer->wide;}
		if(by1 > layer->high){by1 = layer->high;}
		//循环写入数据
		if(layer->color_invisible == LAYER_NOINVISIBLE){
			for(by = by0; by < by1; by++){	//high*2才能写完全部，不然只有一半
				y = layer->y + by;
				for(bx = bx0; bx < bx1; bx++){
					x = layer->x + bx;		
					map[(y * layer_manage->wide + x)] = layer_id;		
				}
			}	
		}else{
			for(by = by0; by < by1; by++){	//high*2才能写完全部，不然只有一半
				y = layer->y + by;
				for(bx = bx0; bx < bx1; bx++){
					x = layer->x + bx;		
					
					if(x0 <= x && x < x1 && y0 <= y && y < y1){
						color = buf[(by * layer->wide) + bx];				
						if(color != layer->color_invisible){
							map[(y * layer_manage->wide + x)] = layer_id;		
						}
					}	
				}
			}	
		}
			
	}		
}	
	
void layer_shift(struct layer *layer, int x, int y)
{
	int old_x = layer->x, old_y = layer->y;
	layer->x = x;
	layer->y = y;
	if(layer->height >= 0){	//如果改变位置后图层为显示状态
		layer_refreshmap(old_x, old_y, old_x + layer->wide, old_y + layer->high, 0);
		layer_refreshmap(x, y, x + layer->wide, y + layer->high, layer->height);
		
		layer_refreshsub(old_x, old_y, old_x + layer->wide, old_y + layer->high, 0, layer->height - 1);
		layer_refreshsub(x, y, x + layer->wide, y + layer->high, layer->height, layer->height);
	}
	
}

void layer_free(struct layer *layer)
{
	if(layer->height >= 0){
		layer_height(layer, -1);	//隐藏
	}
	layer->status = LAYER_UNUSED;
	layer->type = LAYER_TYPE_UNKNOWN;
	
}

void window_change(struct layer *layer, char activate)
{
	int title_len;
	int j, i, wide = layer->wide, high = layer->high;
	char close_button_element;
	uint16_t color_button, color_border;
	uint16_t close_button_color;
	uint16_t *buf = layer->buf;
	
	if(!activate){	// 0
		color_button = COLOR_GREY170;
		color_border = COLOR_GREY;
	}else{
		color_button = COLOR_BLACK;
		color_border = COLOR_WHITE;
	}
	
	//绘制整个窗口
   // draw_square16(buf,wide, 0, 0, wide,high, color_window);
	
	//绘制活动窗口
	draw_square16(buf,wide, 0, 0, wide,1, color_border);
	draw_square16(buf,wide, 0, 0, 1,high, color_border);
	draw_square16(buf,wide, wide-1, 0, 1,high, color_border);
	draw_square16(buf,wide, 0, high-1, wide,1, color_border);
	
	//draw_square16(buf,wide, 4, 28+4, wide-8,high - 8 - 28, COLOR_WHITE);
	//绘制标题
	/*title_len = strlen(layer->title);
	draw_string16(buf,wide, wide - 4 - title_len*8 - 8, 8, COLOR_BLACK, layer->title );*/
	//绘制标题按钮
	static char close_button[15][15] = {
		
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,},
		{0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,},
		{0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,},
		{0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,},
		{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,},
		{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,},
		{0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,},
		{0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,},
		{0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,},
		{0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		
	};
	for(j = 0; j < 15; j++){
		for(i = 0; i < 15; i++){
			close_button_element = close_button[j][i];
			if(close_button_element == 0){
				close_button_color = COLOR_GREY170;
			}else if(close_button_element == 1){
				close_button_color = color_button;
			}
			buf[(j + 4 +1) * wide + (4 + i + 1)] = close_button_color;
		}
	}
}

void window_on(struct layer *layer)
{
	if(layer->type == LAYER_TYPE_WINDOW){
		window_change(layer, 1);
	}
	
	layer->active = true;
	layer_refresh(layer, 0, 0, layer->wide, layer->high);
}
void window_off(struct layer *layer)
{
	if(layer->type == LAYER_TYPE_WINDOW){
		window_change(layer, 0);
	}
	layer->active = false;
	layer_refresh(layer, 0, 0, layer->wide, layer->high);
}


void layer_string(struct layer *layer, uint32_t x,uint32_t y, uint8_t *s, uint16_t color)
{
	draw_string16(layer->buf, layer->wide, x, y, color, s);
	layer_refresh(layer, x, y, x+ strlen(s)*8, y+16);
}

void layer_char(struct layer *layer, uint32_t x,uint32_t y, uint8_t *s, uint16_t color)
{
	char ch[2] = {0,0};
	ch[0] = *s;
	draw_a_word16(layer->buf, layer->wide, x, y, color, &ch);
}

void layer_square(struct layer *layer, uint32_t x,uint32_t y, uint32_t wide,uint32_t high, uint16_t color)
{
	draw_square16(layer->buf, layer->wide, x, y, wide, high, color);
	layer_refresh(layer, x, y, x + wide, y + high);
}

void layer_integer(struct layer *layer, uint32_t x,uint32_t y, uint8_t hex, uint32_t integer, uint16_t color)
{
	switch(hex){
		case 10:
			draw_int16(layer->buf, layer->wide, x, y, color, integer);
			break;
		case 16:
			draw_hex16(layer->buf, layer->wide, x, y, color, integer);
			break;
		default : break;
	}
	layer_refresh(layer, x, y, x + 8*8, y+16);
}


void layer_pix(struct layer *layer, uint32_t x,uint32_t y, uint16_t color)
{
	draw_pix16(layer->buf, layer->wide, x, y, color);
	layer_refresh(layer, x, y, x + 1, y+1);
}
