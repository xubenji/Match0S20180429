#include "kernel/descriptor.h"
#include "driver/clock.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "driver/hd.h"
#include "kernel/syscall.h"
#include "kernel/tss.h"
#include "kernel/syscall.h"
#include "gui/gui.h"
#include "gui/layer.h"
#include "gui/graphic.h"

extern short gdt_limit;
extern int gdt_addr;

struct gdtr gdtr;
irq_handler_t irq_table[NR_IRQ];
void default_irq_handler(int irq);


void init_descriptor()
{
	/*获取gdt得信息*/
	gdtr.limit = (short)(*(&gdt_limit));
	gdtr.addr = (int)(*(&gdt_addr));
	gdtr.addr += 0x80000000;
	put_int(gdtr.addr);
	
	//memcpy(ADR_GDT, gdtr.addr, gdtr.limit);
	
	struct descriptor *gdt = (struct descriptor *) gdtr.addr;
	struct gate       *idt = (struct gate    *)    ADR_IDT;
	
	
	int i;
	
	for (i = 0; i <= 7; i++) {
		set_segment_descriptor(gdt + i, 0, 0, 0);
	}
	
	set_segment_descriptor(gdt + 1, 0xffffffff,   0x00000000, AR_CODE32_ER);
	set_segment_descriptor(gdt + 2, 0xffffffff,   0x00000000, AR_DATA32_RW);
	set_segment_descriptor(gdt + 3, sizeof(tss), (uint32_t )&tss, DA_386TSS);
	set_segment_descriptor(gdt + 4, 0xffffffff, 0x00000000, DA_CR| DA_DPL3|DA_32);
	set_segment_descriptor(gdt + 5, 0xffffffff, 0x00000000, DA_DRW| DA_DPL3|DA_32);
	set_segment_descriptor(gdt + 6, 0xffffffff, 0x00000000, DA_CR| DA_DPL1|DA_32);
	set_segment_descriptor(gdt + 7, 0xffffffff, 0x00000000, DA_DRW| DA_DPL1|DA_32);
  
 
	load_gdtr(gdtr.limit ,gdtr.addr);
	// while(1);
    put_str(">init gdt\n");
	
	init_8259a();
	
	/* IDT初始化 */
	for (i = 0; i <= LIMIT_IDT / 8; i++) {
		set_gate_descriptor(idt + i, 0, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);
	set_gate_descriptor(idt + 0x00, (int )&divide_error, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x01, (int )&single_step_exception, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x02, (int )&nmi, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x03, (int )&breakpoint_exception, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x04, (int )&overflow, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x05, (int )&bounds_check, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x06, (int )&inval_opcode, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x07, (int )&copr_not_available, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x08, (int )&double_fault, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x09, (int )&copr_seg_overrun, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x0a, (int )&inval_tss, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x0b, (int )&segment_not_present, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x0c, (int )&stack_exception, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x0d, (int )&general_protection, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x0e, (int )&page_fault, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x0f, (int )&copr_error, 0x08, DA_386IGate, 0);
	
	/* IDT设定 */
	set_gate_descriptor(idt + 0x20, (int )&IRQ_clock, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x21, (int )&IRQ_keyboard, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x2c, (int )&IRQ_mouse, 0x08, DA_386IGate, 0);
	set_gate_descriptor(idt + 0x20+14, (int )&IRQ_hda, 0x08, DA_386IGate, 0);

	set_gate_descriptor(idt + 0x80, (int )&intrrupt_sys_call, 0x08, DA_386IGate, 3);
	
	for(i = 0; i < NR_IRQ; i++){
		irq_table[i] = default_irq_handler;
	}
	init_syscall();
	
	put_str(">init idt\n");
}


void set_segment_descriptor(struct descriptor *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gate_descriptor(struct gate *gd, uint32_t offset, uint32_t selector, uint32_t ar, uint8_t privilege)
{
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->datacount     = (ar >> 8) & 0xff;
	gd->attr		= ar | (privilege << 5);
	gd->offset_high  = (offset >> 16) & 0xffff;
}

void put_irq_handler(int irq, irq_handler_t handler)
{
	irq_table[irq] = handler;
}
void exception_handler(int esp, int vec_no, int err_code, int eip, int cs, int eflags)
{
	int i;
	char err_description[][64] = {	"#DE Divide Error",
					"#DB RESERVED",
					"—  NMI Interrupt",
					"#BP Breakpoint",
					"#OF Overflow",
					"#BR BOUND Range Exceeded",
					"#UD Invalid Opcode (Undefined Opcode)",
					"#NM Device Not Available (No Math Coprocessor)",
					"#DF Double Fault",
					"    Coprocessor Segment Overrun (reserved)",
					"#TS Invalid TSS",
					"#NP Segment Not Present",
					"#SS Stack-Segment Fault",
					"#GP General Protection",
					"#PF Page Fault",
					"—  (Intel reserved. Do not use.)",
					"#MF x87 FPU Floating-Point Error (Math Fault)",
					"#AC Alignment Check",
					"#MC Machine Check",
					"#XF SIMD Floating-Point Exception"
				};
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 0,  COLOR_RED, "Exception! -->");
	draw_string16(layer_desktop->buf,layer_desktop->wide, 15*8, 0,  COLOR_RED, err_description[vec_no]);
	layer_refresh(layer_desktop, 0, 0, 300, 16);
	put_str("\nException! --> ");
	put_str(err_description[vec_no]);
	
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16,  COLOR_RED, "EFLAGS:");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 8*8, 16,  COLOR_RED, eflags);
	
	draw_string16(layer_desktop->buf,layer_desktop->wide, 200, 16,  COLOR_RED, "CS:");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 200 + 4*8, 16,  COLOR_RED, cs);
	
	draw_string16(layer_desktop->buf,layer_desktop->wide, 350, 16,  COLOR_RED, "EIP:");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 350 + 5*8, 16,  COLOR_RED, eip);
	layer_refresh(layer_desktop, 0, 16, 500, 16*2);
	
	
	put_str("\nEFLAGS:");
	put_int(eflags);
	put_str("CS:");
	put_int(cs);
	put_str( "EIP:");
	put_int(eip);
	
	
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*3,  COLOR_RED, "esp:");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 0 + 12*8, 16*3,  COLOR_RED, esp);
	layer_refresh(layer_desktop, 0, 16*3, 200, 16*4);
		
	
	
	if(err_code != 0xFFFFFFFF){
		put_str("Error code:");
		put_int(err_code);
		draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*2,  COLOR_RED, "Error code:");
		draw_hex16(layer_desktop->buf,layer_desktop->wide, 0 + 12*8, 16*2,  COLOR_RED, err_code);
		layer_refresh(layer_desktop, 0, 16*2, 200, 16*3);
		
	}

}


void default_irq_handler(int irq)
{
	put_str("!default_irq_handler!");
	put_int(irq);
	put_str("\n");
}

