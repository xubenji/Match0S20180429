#include "driver/keyboard.h"
#include "kernel/descriptor.h"
#include "kernel/io.h"
#include "driver/vga.h"
#include "lib/fifo.h"
#include "driver/keymap.h"
#include "gui/gui.h"
#include "gui/graphic.h"
#include "gui/layer.h"
#include "driver/console.h"
#include "kernel/syscall.h"


struct FIFO32 key_fifo;

int key_buf[KEYBOARD_FIFO_LEN];
int syscall_keyboard_char;

uint8_t get_byte_from_kbuf() ;
static	int	code_with_E0 = 0;
static	int	shift_l;	/* l shift state */
static	int	shift_r;	/* r shift state */
static	int	alt_l;		/* l alt state	 */
static	int	alt_r;		/* r left state	 */
static	int	ctrl_l;		/* l ctrl state	 */
static	int	ctrl_r;		/* l ctrl state	 */
static	int	caps_lock;	/* Caps Lock	 */
static	int	num_lock;	/* Num Lock	 */
static	int	scroll_lock;	/* Scroll Lock	 */
static	int	column;

static void kb_wait();
static void kb_ack();
static void set_leds();

void key_char_process(uint32_t key);
void put_key(uint32_t key);
void wait_KBC_sendready(void);

extern struct layer *active_layer;
extern struct task *task_console;

void keyboard_handler(int32_t irq)
{
	uint8_t scan_code = io_in8(KB_DATA);
	fifo32_put(&key_fifo, scan_code);
	
	//keyboard_read();
}
#define PORT_KEYSTA				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void init_keyboard()
{
	fifo32_init(&key_fifo, KEYBOARD_FIFO_LEN, key_buf);
	shift_l	= shift_r = 0;
	alt_l	= alt_r   = 0;
	ctrl_l	= ctrl_r  = 0;
	
	caps_lock   = 0;
	num_lock    = 1;
	scroll_lock = 0;
	
	syscall_keyboard_char = -1;
	
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	
	put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
	enable_irq(KEYBOARD_IRQ);
	
	put_str(">init keyboard\n");
	
}


void wait_KBC_sendready(void)
{
	/*等待键盘控制电路准备完毕*/
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void task_keyboard_entry()
{
	int i = 0;
	struct task *task = task_current();
	//printf("Driver of keyboard is running.\n");
	for(;;){
		if(fifo32_status(&key_fifo) == 0){
			task_deprive(task);
		} else {
			if(fifo32_status(&key_fifo) != 0){
				keyboard_read();
				/*i++;
				draw_square16(layer_win->buf, layer_win->wide, 4, 28+4 + 16, 8*10,16, COLOR_WHITE);
				draw_hex16(layer_win->buf, layer_win->wide, 4, 28+4 + 16,COLOR_BLACK, sys_get_ticks());
				layer_refresh(layer_win, 4, 28+4 + 16, 4 + 8*10, 28+4 + 16 + 16);*/
				/*if(syscall_keyboard_char == '\t'){
					if(i == 0){
						i = 1;
						draw_window(layer_win->buf,layer_win->wide, layer_win->high, "win", 1);
						draw_window(layer_console->buf,layer_console->wide, layer_console->high, "console", 0);
					}else{
						i = 0;
						draw_window(layer_win->buf,layer_win->wide, layer_win->high, "win", 0);
						draw_window(layer_console->buf,layer_console->wide, layer_console->high, "console", 1);
					}
					layer_refresh(layer_win, 0, 0, layer_win->wide, layer_win->high);
					layer_refresh(layer_console, 0, 0, layer_console->wide, layer_console->high);
					syscall_keyboard_char = -1;
				}*/
			}
		}
		
	}
	
	
	
}

void keyboard_read()
{
	uint8_t scan_code;
	int make;
	
	uint32_t key = 0;
	uint32_t* keyrow;

		code_with_E0 = 0;

		scan_code = get_byte_from_kbuf();
		
		if(scan_code == 0xe1){
			int i;
			uint8_t pausebrk_scode[] = {0xE1, 0x1D, 0x45,
					       0xE1, 0x9D, 0xC5};
			int is_pausebreak = 1;
			for(i=1;i<6;i++){
				if (get_byte_from_kbuf() != pausebrk_scode[i]) {
					is_pausebreak = 0;
					break;
				}
			}
			if (is_pausebreak) {
				key = PAUSEBREAK;
			}
		} else if(scan_code == 0xe0){
			scan_code = get_byte_from_kbuf();

			/* PrintScreen 被按下 */
			if (scan_code == 0x2A) {
				if (get_byte_from_kbuf() == 0xE0) {
					if (get_byte_from_kbuf() == 0x37) {
						key = PRINTSCREEN;
						make = 1;
					}
				}
			}
			/* PrintScreen 被释放 */
			if (scan_code == 0xB7) {
				if (get_byte_from_kbuf() == 0xE0) {
					if (get_byte_from_kbuf() == 0xAA) {
						key = PRINTSCREEN;
						make = 0;
					}
				}
			}
			/* 不是PrintScreen, 此时scan_code为0xE0紧跟的那个值. */
			if (key == 0) {
				code_with_E0 = 1;
			}
		}if ((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
			make = (scan_code & FLAG_BREAK ? 0 : 1);

			/* 先定位到 keymap 中的行 */
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];
			
			column = 0;
			int caps = shift_l || shift_r;
			if (caps_lock) {
				if ((keyrow[0] >= 'a') && (keyrow[0] <= 'z')){
					caps = !caps;
				}
			}
			if (caps) {
				column = 1;
			}

			if (code_with_E0) {
				column = 2;
			}
			
			key = keyrow[column];
			
			switch(key) {
			case SHIFT_L:
				shift_l = make;
				break;
			case SHIFT_R:
				shift_r = make;
				break;
			case CTRL_L:
				ctrl_l = make;
				break;
			case CTRL_R:
				ctrl_r = make;
				break;
			case ALT_L:
				alt_l = make;
				break;
			case ALT_R:
				alt_l = make;
				break;
			case CAPS_LOCK:
				if (make) {
					caps_lock   = !caps_lock;
					set_leds();
				}
				break;
			case NUM_LOCK:
				if (make) {
					num_lock    = !num_lock;
					set_leds();
				}
				break;
			case SCROLL_LOCK:
				if (make) {
					scroll_lock = !scroll_lock;
					set_leds();
				}
				break;	
			default:
				break;
			}
			
			if (make) { /* 忽略 Break Code */
				int pad = 0;

				/* 首先处理小键盘 */
				if ((key >= PAD_SLASH) && (key <= PAD_9)) {
					pad = 1;
					switch(key) {
					case PAD_SLASH:
						key = '/';
						break;
					case PAD_STAR:
						key = '*';
						break;
					case PAD_MINUS:
						key = '-';
						break;
					case PAD_PLUS:
						key = '+';
						break;
					case PAD_ENTER:
						key = ENTER;
						break;
					default:
						if (num_lock &&
						    (key >= PAD_0) &&
						    (key <= PAD_9)) {
							key = key - PAD_0 + '0';
						}
						else if (num_lock &&
							 (key == PAD_DOT)) {
							key = '.';
						}
						else{
							switch(key) {
							case PAD_HOME:
								key = HOME;
								break;
							case PAD_END:
								key = END;
								break;
							case PAD_PAGEUP:
								key = PAGEUP;
								break;
							case PAD_PAGEDOWN:
								key = PAGEDOWN;
								break;
							case PAD_INS:
								key = INSERT;
								break;
							case PAD_UP:
								key = UP;
								break;
							case PAD_DOWN:
								key = DOWN;
								break;
							case PAD_LEFT:
								key = LEFT;
								break;
							case PAD_RIGHT:
								key = RIGHT;
								break;
							case PAD_DOT:
								key = DELETE;
								break;
							default:
								break;
							}
						}
						break;
					}
				}
				
				key |= shift_l	? FLAG_SHIFT_L	: 0;
				key |= shift_r	? FLAG_SHIFT_R	: 0;
				key |= ctrl_l	? FLAG_CTRL_L	: 0;
				key |= ctrl_r	? FLAG_CTRL_R	: 0;
				key |= alt_l	? FLAG_ALT_L	: 0;
				key |= alt_r	? FLAG_ALT_R	: 0;
				key |= pad      ? FLAG_PAD      : 0;
				
				//put_str(&key);
				key_char_process( key);
			}
			
		}
	//milli_delay(50);
}


void key_char_process(uint32_t key)
{
	char output[2] = {'\0', '\0'};

	if (!(key & FLAG_EXT)) {
		put_key( key);
	} else {
		int raw_code = key & MASK_RAW;
		switch(raw_code) {
			case ENTER:
				put_key('\n');
				break;
			case BACKSPACE:
				put_key( '\b');
				break;
			case TAB:
				put_key( '\t');
				
				
				
				break;
			case F12:
				put_key( F12);
				if(layer_manage->top > 2){
					layer_height(layer_manage->layers_ptr[1], layer_manage->top - 1);
					
				}
				break;
			default:
				break;
		}
	}
}

void put_key(uint32_t key)
{
	struct task *task;
	int i;
	//put_char(key);
	//syscall_keyboard_char = key;
	
	if(active_layer == task_console->layer || task_console->layer->active == true){
		fifo32_put(&console.fifo, key);
	}else{
		for(i = 0; i < task_running; i++){
			if(tasks_ptr[i]->layer == active_layer && tasks_ptr[i]->layer->type == LAYER_TYPE_WINDOW){
				task = tasks_ptr[i];
				task->key_data = key;
				break;
			}
			
		}
		
		///fifo32_put(&syscall_info.fifo, key);
		
	}
	//fifo32_put(&syscall_info.fifo, key);
	
	
}

uint8_t get_byte_from_kbuf()       /* 从键盘缓冲区中读取下一个字节 */
{
        uint8_t scan_code;
		//while(fifo32_status(&key_fifo) == 0){};
		
        io_cli();
		scan_code = fifo32_get(&key_fifo);
        io_sti();

        return scan_code;
}

static void kb_wait()	/* 等待 8042 的输入缓冲区空 */
{
	uint8_t kb_stat;

	do {
		kb_stat = io_in8(KB_CMD);
	} while (kb_stat & 0x02);
}

static void kb_ack()
{
	uint8_t kb_read;

	do {
		kb_read = io_in8(KB_DATA);
	} while (kb_read =! KB_ACK);
}

static void set_leds()
{
	uint8_t leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock;

	kb_wait();
	io_out8(KB_DATA, LED_CODE);
	kb_ack();

	kb_wait();
	io_out8(KB_DATA, leds);
	kb_ack();
}

