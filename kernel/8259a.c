#include "driver/vga.h"
#include "kernel/8259a.h"
#include "kernel/io.h"

void init_8259a(void)
{
	io_out8(PIC0_IMR,  0xff  );
	io_out8(PIC1_IMR,  0xff  );

	io_out8(PIC0_ICW1, 0x11  );
	io_out8(PIC0_ICW2, 0x20  );
	io_out8(PIC0_ICW3, 1 << 2);
	io_out8(PIC0_ICW4, 0x01  );

	io_out8(PIC1_ICW1, 0x11  );
	io_out8(PIC1_ICW2, 0x28  );
	io_out8(PIC1_ICW3, 2     );
	io_out8(PIC1_ICW4, 0x01  );

	io_out8(PIC0_IMR,  0xff  );
	io_out8(PIC1_IMR,  0xff  );
	
	put_str(">init 8259a\n");
	return;
}
