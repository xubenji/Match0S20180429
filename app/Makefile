###################
#Match OS Makefile#
###################

#Made by Hu Zicheng

#the kernel entry
APP_ENTRY = 0x00000000

#kernel is on the 9th sector
#now kernel only use 256 sector(256*512/1024 = 128KB)
APP_SECTOR_OFFEST = 260
APP_SECTORS = 10


HTML_SECTOR_OFFEST = 300
HTML_SECTORS = 10

#the tool path
NASM 		= nasm
CC			= gcc
LD 			= ld
QEMU 		= qemu

#the flags
ASM_KERNEL_FLAGS	= -I ../include/ -f elf
C_KERNEL_FLAGS	= -I ../include/ -c -fno-builtin
LDFLAGS		= --oformat binary -N -e _start -Ttext $(APP_ENTRY)

#the file path
APP_BIN = app.bin
HTML_FILE = html.html
MATCH_IMG = ../image/hd.img

#the objs
OBJS =  _start.o app.o syscall.o string.o

#finall destination is all
.PHONY:all

#all is clean temporary files, compile files, write file into disk(floppy)
all: clean compile write_disk
#compile files
compile: $(APP_BIN)
#clean files
clean:
	-rm app.o
	-rm _start.o
	-rm *.bin
#write file into disk
write_disk:
	dd if=$(APP_BIN) of=$(MATCH_IMG) bs=512 seek=$(APP_SECTOR_OFFEST) count=$(APP_SECTORS) conv=notrunc
	dd if=$(HTML_FILE) of=$(MATCH_IMG) bs=512 seek=$(HTML_SECTOR_OFFEST) count=$(HTML_SECTORS) conv=notrunc
#clean temporary files
$(APP_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(APP_BIN) $(OBJS)
	
app.o : app.c
	$(CC) $(C_KERNEL_FLAGS) -o $@ $<
_start.o : _start.asm
	$(NASM) $(ASM_KERNEL_FLAGS) -o $@ $<
