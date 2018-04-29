[bits 32]
[section .text]
global asm_rdtsc
global asm_cpuid

asm_rdtsc:	; void asm_rdtsc(int *high, int *low);
	pushad
	db		0x0F, 0x31			; RDTSC
	mov edi, [esp + 36]
	mov dword [edi], EDX
	mov edi, [esp + 40]
	mov dword [edi], eax
	popad
	ret

asm_cpuid:	; void asm_cpuid(int id_eax, int id_ecx, int *eax, int *ebx, int *ecx, int *edx);
	pushad
	mov		eax,[esp+36]		; id_eax
	mov		ecx,[esp+40]		; id_ecx
	db		0x0F, 0xA2			; CPUID
	mov edi,[esp+44]
	mov [edi], eax
	mov edi,[esp+48]
	mov dword [edi], ebx
	mov edi,[esp+52]
	mov dword [edi], ecx
	mov edi,[esp+56]
	mov dword [edi], edx
;
	popad
	ret