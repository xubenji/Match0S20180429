KERNEL_STACK_TOP EQU 0x8009fc00

[bits 32]
extern kernel_entry	
global gdtr
global gdt_limit
global gdt_addr

global _start
_start:
	;put 'K'
	mov dword [0x800b8000+6], 'K'
	mov dword [0x800b8000+7], 0X06
	;init all segment registeres
	mov ax, 0x10	;the data 
	mov ds, ax 
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
	mov ss, ax 
	mov esp, KERNEL_STACK_TOP
	
	sgdt [gdtr]
	
	call kernel_entry					;into c mian
	
	;ud2
stop_run:
	hlt
	jmp stop_run
jmp $	


gdtr:
gdt_limit: dw 0
gdt_addr: dd 0

dd		0xaaaaaaaa
dd		0x00000000		
	
	