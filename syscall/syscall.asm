[bits 32]
[section .text]

INT_VECTOR_SYS_CALL equ 0x80


;系统调用的idx要和sys_call_table一样
NR_SYS_CALL equ 100
SYS_CALL_INFO equ 0			;系统调用信息的位置
SYS_CALL_FUNC equ (SYS_CALL_INFO+NR_SYS_CALL/2)		;系统调用函数的位置


;参数顺序：eax, ebx, edx, ecx, esi, edi
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global getTicks
getTicks:
	mov eax, SYS_CALL_FUNC+0
	int INT_VECTOR_SYS_CALL
	ret
	
global usr_printf
usr_printf:
	mov eax, SYS_CALL_FUNC+1
	mov ebx, [esp + 4]		;第一个参数
	mov edx, [esp + 8]		;第二个参数
	int INT_VECTOR_SYS_CALL
	ret

global cpuCpuid
cpuCpuid:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+34
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	int INT_VECTOR_SYS_CALL
	;回复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	ret
	
global readSector
readSector:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+35
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	int INT_VECTOR_SYS_CALL
	;回复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	ret
	
global writeSector
writeSector:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+36
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	int INT_VECTOR_SYS_CALL
	;回复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	ret
	
global cpuRdtsc
cpuRdtsc:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+33
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	int INT_VECTOR_SYS_CALL
	;回复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	ret
	
global creatWindow
creatWindow:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+17
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	mov edi, [esp + 20+4*5]
	int INT_VECTOR_SYS_CALL
	;回复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	

global drawSquare
drawSquare:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+18
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	mov edi, [esp + 20+4*5]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	
	
global drawString
drawString:
	;保存寄存器
	
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+20
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	
	
global drawInt
drawInt:
	;保存寄存器
	
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+30
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret		
global drawPoint
drawPoint:
	;保存寄存器
	
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+21
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret		

global fillColor
fillColor:
	;保存寄存器
	
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+19
	mov ebx, [esp + 20+4*1]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	
global fontColor
fontColor:
	;保存寄存器
	
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+29
	mov ebx, [esp + 20+4*1]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret		
global refreshWindow
refreshWindow:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+22
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	mov edi, [esp + 20+4*5]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret		
global drawLine
drawLine:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+23
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	mov edi, [esp + 20+4*5]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret

global closeWindow
closeWindow:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+24
	mov ebx, [esp + 20+4*1]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	

global getChar
getChar:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+25
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	

global closeTask
closeTask:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+26
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret		
	
global drawHex
drawHex:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+27
	mov ebx, [esp + 20+4*1]
	mov edx, [esp + 20+4*2]
	mov ecx, [esp + 20+4*3]
	mov esi, [esp + 20+4*4]
	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret


global milliDelay
milliDelay:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+28
	mov ebx, [esp + 20+4*1]

	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret

global malloc
malloc:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+31
	mov ebx, [esp + 20+4*1]

	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret	
	
global mfree
mfree:
	;保存寄存器
	push edi
	push esi
	push ecx
	push edx
	push ebx
	
	mov eax, SYS_CALL_FUNC+32
	mov ebx, [esp + 20+4*1]

	int INT_VECTOR_SYS_CALL
	;恢复寄存器
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop edi
	
	ret		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	
global set_syscall_pages
set_syscall_pages:
	mov eax, SYS_CALL_INFO+7
	mov ebx, [esp + 4]
	int INT_VECTOR_SYS_CALL
	ret
	
global set_syscall_address
set_syscall_address:
	mov eax, SYS_CALL_INFO+8
	mov ebx, [esp + 4]
	int INT_VECTOR_SYS_CALL
	ret


	