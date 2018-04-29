###################
#Match OS Makefile#
###################

#Made by yanjie XU & Zicheng HU
#Degubber by ling TANG


#the kernel entry
KERNEL_ENTRY = 0x80010000

#load is on the 1st sector
#this time we use 8 sectors for load.it's 1kb(4096 bytes)
LOAD_SECTOR_OFFSET = 1		
LOAD_SECTORS = 8

#kernel is on the 9th sector
#now kernel only use 256 sector(256*512/1024 = 128KB)
KERNEL_SECTOR_OFFEST = 9
KERNEL_SECTORS = 256

ICO_SECTOR_OFFEST = 4000
ICO_SECTORS = 2820

WIN_SECTOR_OFFEST = 7000
WIN_SECTORS = 450

#the tool path
NASM 		= nasm
CC			= gcc
LD 			= ld
QEMU 		= qemu

#stack
NOSTACK         =-fno-stack-protector -shared

#bochs
b:
	bochs -f image/bochsrc

#clean BOCHSRC
cb:
	rm -f image/bochsout.txt
	rm -f image/c.img.lock
	


#the flags
ASM_BOOT_FLAGS	= -I boot/include/
ASM_KERNEL_FLAGS	= -I include/ -f elf
C_KERNEL_FLAGS	= -I include/ -c -fno-builtin $(NOSTACK)
LDFLAGS		= --oformat binary -N -e _start -Ttext $(KERNEL_ENTRY) $(NOSTACK)

#the file path
BOOT_BIN = boot/boot.bin
LOAD_BIN = boot/load.bin
KERNEL_FILE = kernel/KERNEL
MATCH_IMG = image/Match.img
HD0_IMG = image/c.img

ICO_BMP = src/22.bmp
WIM_BMP = src/ww.bmp

#the objs
OBJS =  kernel/_start.o kernel/entry.o driver/vga.o lib/string.o\
		kernel/descriptor.o kernel/io.o kernel/8259a.o kernel/interrupt.o\
		driver/clock.o driver/keyboard.o lib/bitmap.o lib/fifo.o kernel/memory.o\
		kernel/tss.o kernel/syscall.o syscall/syscall.o print/printk.o\
		print/vsprintf.o syscall/printf.o kernel/task.o driver/video.o\
		gui/graphic.o gui/font.o gui/layer.o gui/gui.o driver/mouse.o\
		driver/console.o driver/cmos.o command/cmd_cls.o command/cmd_os.o\
		command/cmd_help.o command/cmd_ptask.o command/cmd_mem.o application/application.o\
		application/app_walk.o application/app_2048.o application/app_test.o\
		kernel/ards.o lib/list.o driver/hd.o application/app_cpuid.o kernel/cpu.o\
		kernel/asm.o command/cmd_hd.o command/cmd_ie.o kernel/system.o gui/desktop.o\
		fs/fs.o command/cmd_ls.o lib/bmp.o data/bmp_data.o

#finall destination is all
.PHONY: all

#all is clean temporary files, compile files, write file into disk(floppy)
all: compile write_disk
#compile files
compile: $(BOOT_BIN) $(LOAD_BIN) $(KERNEL_FILE)
#write file into disk
write_disk:
	dd if=$(BOOT_BIN) of=$(MATCH_IMG) bs=512 count=1 conv=notrunc
	dd if=$(LOAD_BIN) of=$(MATCH_IMG) bs=512 seek=$(LOAD_SECTOR_OFFSET) count=$(LOAD_SECTORS) conv=notrunc
	dd if=$(KERNEL_FILE) of=$(MATCH_IMG) bs=512 seek=$(KERNEL_SECTOR_OFFEST) count=$(KERNEL_SECTORS) conv=notrunc
	dd if=$(ICO_BMP) of=$(HD0_IMG) bs=512 seek=$(ICO_SECTOR_OFFEST) count=$(ICO_SECTORS) conv=notrunc
	dd if=$(WIM_BMP) of=$(HD0_IMG) bs=512 seek=$(WIN_SECTOR_OFFEST) count=450 conv=notrunc
	
#clean temporary files
clean:
	-rm kernel/KERNEL 
	-rm kernel/*.o
	-rm lib/*.o
	-rm driver/*.o
	-rm syscall/*.o
	-rm gui/*.o
	-rm command/*.o
	-rm application/*.o
	-rm print/*.o
	-rm boot/*.bin
	-rm image/bochsout.txt
	-rm image/c.img.lock
	
cimg:
	-rm image/c.img.lock
#only clean kernel
clean_kernel:
	-rm kernel/KERNEL 
	-rm kernel/*.o
run: all
	bochs -q -f image/bochsrc
	#$(QEMU) -fda $(MATCH_IMG)
debug:
	bochsdbg -q -f bochsrc.bxrc
#$(QEMU) -fda $(MATCH_IMG)

boot:
$(BOOT_BIN):boot/boot.asm
	$(NASM) $(ASM_BOOT_FLAGS) -o $@ $<
$(LOAD_BIN) : boot/load.asm
	$(NASM) $(ASM_BOOT_FLAGS) -o $@ $<

#the specific compilation content

$(KERNEL_FILE): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_FILE) $(OBJS)
############
#kernel file
############	
kernel/_start.o : kernel/_start.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
kernel/entry.o : kernel/entry.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

kernel/io.o : kernel/io.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<

kernel/memory.o : kernel/memory.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

kernel/tss.o : kernel/tss.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<		

kernel/syscall.o : kernel/syscall.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

kernel/task.o : kernel/task.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<	
kernel/8259a.o : kernel/8259a.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

kernel/interrupt.o : kernel/interrupt.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
	
kernel/descriptor.o : kernel/descriptor.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
kernel/ards.o : kernel/ards.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
kernel/cpu.o : kernel/cpu.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
kernel/asm.o : kernel/asm.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
kernel/system.o : kernel/system.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<	
	
############
#driver file
############
driver/vga.o : driver/vga.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
driver/clock.o : driver/clock.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

driver/keyboard.o : driver/keyboard.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

driver/video.o : driver/video.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

driver/mouse.o : driver/mouse.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

driver/console.o : driver/console.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
driver/cmos.o : driver/cmos.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
driver/hd.o : driver/hd.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

############	
#lib file 	
############
lib/string.o : lib/string.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

lib/bitmap.o : lib/bitmap.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
lib/fifo.o : lib/fifo.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
lib/list.o : lib/list.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
lib/bmp.o : lib/bmp.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
		
	
############
#syscall file
############ 	
syscall/syscall.o : syscall/syscall.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<

syscall/printf.o : syscall/printf.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
	
############	
#print file 
############

print/vsprintf.o : print/vsprintf.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<	

print/printk.o : print/printk.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<	

############	
#gui file
############

gui/graphic.o : gui/graphic.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
gui/font.o : gui/font.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
gui/layer.o : gui/layer.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
gui/gui.o : gui/gui.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
gui/desktop.o : gui/desktop.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
############
#fs file
############ 	
fs/fs.o : fs/fs.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
	
############	
#command file
############

command/cmd_cls.o : command/cmd_cls.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

command/cmd_os.o : command/cmd_os.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

command/cmd_help.o : command/cmd_help.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
command/cmd_ptask.o : command/cmd_ptask.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
	
command/cmd_mem.o : command/cmd_mem.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
command/cmd_hd.o : command/cmd_hd.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
command/cmd_ie.o : command/cmd_ie.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
command/cmd_ls.o : command/cmd_ls.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

############	
#app file
############

application/application.o : application/application.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<

application/app_walk.o : application/app_walk.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
application/app_2048.o : application/app_2048.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
application/app_test.o : application/app_test.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
application/app_cpuid.o : application/app_cpuid.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
############	
#data file
############
data/bmp_data.o : data/bmp_data.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
