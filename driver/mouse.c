#include "driver/mouse.h"
#include "driver/keyboard.h"
#include "driver/video.h"
#include "kernel/8259a.h"
#include "lib/fifo.h"
#include "gui/gui.h"
#include "gui/layer.h"
#include "kernel/memory.h"

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
struct mouse mouse;
extern struct task *task_mouse;
struct layer *active_layer;
void init_mouse()
{
	mouse.x = mouse.new_x = video_info.wide/2;		//鼠标信息初始化
	mouse.y = mouse.new_y = video_info.high/2;
	
	//初始化鼠标数据缓冲
	fifo32_init(&mouse.fifo, MOUSE_FIFO_BUF_LEN, mouse.mouse_fifo_buf);
	//初始化鼠标中断
	put_irq_handler(MOUSE_IRQ, mouse_handler);
	//打开从片
	enable_irq(CASCADE_IRQ);
	//打开鼠标中断
	enable_irq(MOUSE_IRQ);
	//激活鼠标前要设定键盘相关信息
	enable_mouse(&mouse);
}

void enable_mouse(struct mouse *mouse)
{
	/* 激活鼠标 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/*键盘控制器返回ACK(0xfa)*/
	mouse->phase = 0;
	return;
}

void mouse_handler(int32_t irq)
{
	uint8_t data;
	io_out8(PIC1_OCW2, 0x64);	/* IRQ-12接受通知*/
	io_out8(PIC0_OCW2, 0x62);	/* IRQ-02接受通知*/
	data = io_in8(PORT_KEYDAT);
	fifo32_put(&mouse.fifo, data);
	//task_mouse->status = TASK_WAKEUP;
	//put_str("mouse\n");
	return;
}

void task_mouse_entry()
{
	//printf("Driver of mouse is running.\n");
	int i, j, l, x, y;

	struct task *task = task_current();
	struct layer *layer = 0;	//活动中的图层
	bool window_move_mode = false;		//改变窗口的位置
	bool window_change_mode = false;	//改变窗口的状态
	struct layer *catch_layer;	//抓住中的图层
	
	int distance_x = 0, distance_y = 0;		//计算鼠标点击标题栏时鼠标和图层的距离
	
	active_layer = layer_console;
	uint16_t *buf_catching;
	struct layer *catching_layer = layer_alloc(LAYER_TYPE_WINDOW);	//移动中，而且还是隐形的新图层
	char is_catching = 0;
	for(;;){
		//io_cli();
		task = task_current();
		if(fifo32_status(&mouse.fifo) == 0){
			if(mouse.new_x >= 0){	//当鼠标位置不为-1就说明改变了，就要重新刷新过
				layer_shift(layer_mouse, mouse.new_x, mouse.new_y);
				mouse.new_x = -1;
				
			}else{
				
				task_deprive(task);
			}
			
		} else {
			if(fifo32_status(&mouse.fifo) != 0){
				//task->ticks = task->priority;
				i = fifo32_get(&mouse.fifo);
				//io_sti();
				if (mouse_read(&mouse, i) != 0) {
					
					mouse.x += mouse.x_increase;
					mouse.y += mouse.y_increase;
					if (mouse.x < 0) {
						mouse.x = 0;
					}
					if (mouse.y < 0) {
						mouse.y = 0;
					}
					if (mouse.x > layer_manage->wide - 1) {
						mouse.x = layer_manage->wide - 1;
					}
					if (mouse.y > layer_manage->high - 1) {
						mouse.y = layer_manage->high- 1;
					}
					//layer_shift(layer_mouse, mouse.x, mouse.y);
					mouse.new_x = mouse.x;
					mouse.new_y = mouse.y;
					
					if((mouse.button & 0x01) != 0){
						//layer_shift(layer_win, mouse.x, mouse.y);
						if(window_move_mode == false){
							for(j = layer_manage->top - 1; j > 0; j--){
								layer = layer_manage->layers_ptr[j];
								x = mouse.x - layer->x;		//鼠标位置和图层位置的距离
								y = mouse.y - layer->y;
								distance_x = mouse.x - layer->x;
								distance_y = mouse.y - layer->y;
								if(0 <= x && x < layer->wide && 0 <= y && y < layer->high){
									if(layer->buf[y * layer->wide + x] != layer->color_invisible){
										layer_height(layer, layer_manage->top - 1);
										if(window_change_mode == false && active_layer != layer ){
											if(active_layer->type == LAYER_TYPE_WINDOW || active_layer->type == LAYER_TYPE_NOTMOVE){
												window_off(active_layer);
												active_layer = layer;
												window_on(active_layer);

												window_change_mode = true;
												
											}
											
											
										}
										if(5 <= x && x < 5+15 && 5 <= y && y < 5+15 && layer->type == LAYER_TYPE_WINDOW){
											task = active_layer->task;
											task_close(task);
											
											console_tip("Close window -MOUSE");
											
											if(active_layer == layer){
												active_layer = layer_console;
											}
											layer_free(layer);
										}
										if(4 <= x && x < layer->wide - 4 && 4 <= y && y < 24 && layer->type == LAYER_TYPE_WINDOW){
											window_move_mode = true;
											/*x = mouse.x;
											y = mouse.y;*/
											catch_layer = layer;	//定位选取图层
											layer_height(catch_layer, -1);	//隐藏
											if(is_catching == 0){
												is_catching = 1;
												buf_catching = sys_malloc(catch_layer->wide * catch_layer->high * 2);
												layer_init(catching_layer, buf_catching, catch_layer->wide, catch_layer->high, LAYER_INVISIBLE);
												draw_moving_window(catching_layer->buf, catch_layer->wide ,catch_layer->high,LAYER_INVISIBLE);
												layer_shift(catching_layer,mouse.x - distance_x, mouse.y - distance_y);
												layer_height(catching_layer,layer_manage->top);
												layer_refresh(catching_layer, 0, 0, catching_layer->wide, catching_layer->high);
											}										
										}
										
										
										
										break;
									}
									
								}
								
							}
						}else{
							/*distance_x = mouse.x - x;
							distance_y = mouse.y - y;*/
							//layer_shift(layer, layer->x + distance_x, layer->y + distance_y);
							//layer_shift(layer, mouse.x, mouse.y);
							if(is_catching == 1){	//如果抓取图层就显示移动
								layer_shift(catching_layer,mouse.x - distance_x , mouse.y - distance_y);
								layer_shift(layer_mouse, mouse.x, mouse.y);
							}
							
							/*x = mouse.x;
							y = mouse.y;*/
						}
					}else{
						//没有按下左键
						if(window_move_mode == true){
						
							layer_height(catching_layer,-1);	//隐藏移动层
							
							layer_shift(layer, mouse.x - distance_x, mouse.y - distance_y);	//被移动图层位置改变
							window_move_mode = false;
							layer_height(catch_layer, layer_manage->top);	//显示被移动层
							catch_layer = layer_mouse;	//取消对窗口的定位
							is_catching = 0;		//没有抓取图层
						}
						window_change_mode = false;	
						
						//layer_shift(layer, mouse.x, mouse.y);
					}
					//printf("[%d %d]", mx, my);
				}
			}
		}
	}
}

int mouse_read(struct mouse *mouse, unsigned char data)
{
	if (mouse->phase == 0) {
		if (data == 0xfa) {
			mouse->phase = 1;
		}
		return 0;
	}
	if (mouse->phase == 1) {
		if ((data & 0xc8) == 0x08) {
			mouse->read_buf[0] = data;
			mouse->phase = 2;
		}
		return 0;
	}
	if (mouse->phase == 2) {
		mouse->read_buf[1] = data;
		mouse->phase = 3;
		return 0;
	}
	if (mouse->phase == 3) {
		mouse->read_buf[2] = data;
		mouse->phase = 1;
		mouse->button = mouse->read_buf[0] & 0x07;
		mouse->x_increase = mouse->read_buf[1];
		mouse->y_increase = mouse->read_buf[2];
		if ((mouse->read_buf[0] & 0x10) != 0) {
			mouse->x_increase |= 0xffffff00;
		}
		if ((mouse->read_buf[0] & 0x20) != 0) {
			mouse->y_increase |= 0xffffff00;
		}
		mouse->y_increase = - mouse->y_increase;
		return 1;
	}
	return -1; 
}