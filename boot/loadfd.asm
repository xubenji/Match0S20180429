KERNEL_SECTOR_OFFSET EQU 9		;kernel is on the 9th sector
KERNEL_SECTORS EQU 256	;now kernel only use 256 sector(256*512/1024 = 128KB)
;0x7c00~0x7e00	boot.bin	512byte 1sector
;0x70000~0x71000	load.bin	1kb 8sector	
;0x71000~0x80000	kernel.bin	60kb 120sector	

;0x111 640*480*16bit 
;0x114 800*600*16bit 
;0x117 1024*768*16bit 
;0x11a 1280*1024*16bit 

MODE_640_480 EQU 0x111
MODE_800_600 EQU 0x114
MODE_10240_7680 EQU 0x117
MODE_1280_1024 EQU 0x11A

VBEMODE	EQU	MODE_800_600			; 800*600*16bit



;其实是0x600*16+0x0000，因为偏移地址
ARDS_BUF	EQU		0x0004	;ards buf 地址（0x6004）
ARDS_NR		EQU		0x0000	;ards 的数量（0x6000）
;画面信息16字节
VCOLOR	EQU		0			; 颜色数
XWIDTH	EQU		2			; x的分辨率	宽度
YHEIGHT	EQU		4			; y的分辨率	高度
VRAM	EQU		6			; VRAM 地址

	org 0x70000		;load was loaded at 0x10000

	mov ax, cs
	mov ds, ax 
	mov es, ax 
	mov ss, ax 
	mov sp, 0	;ss=0x1000, sp=0
	
	;put 'L'
	mov ax, 0xb800
	mov es, ax 
	mov al, [char]
	mov byte [es:2],al
	mov byte [es:3],0X06
	
;we read load from sector 1(0 is first) width 8 sectors to 0x11000
	;read 128 sectors
	mov ax, 0x1000
	mov es, ax
	
	mov eax, KERNEL_SECTOR_OFFSET
	xor ebx, ebx 
	mov ecx, 128
	call rd_disk_m_32
	;read 128 sectors 
	mov ax, 0x2000
	mov es, ax
	
	mov eax, KERNEL_SECTOR_OFFSET+128
	xor ebx, ebx 
	mov ecx, 128
	call rd_disk_m_32
	
;we read load from sector 1(0 is first) width 8 sectors to 0x11000
	mov ax, 0x1000
	mov es, ax 
	
	xor bx, bx 
	mov si, KERNEL_SECTOR_OFFSET
	mov cx, 128
rd_kern1:	
	call read_floppy_sector
	add bx, 512
	inc si 
	loop rd_kern1
	
	mov ax, 0x2000
	mov es, ax 
	
	xor bx, bx 
	mov si, KERNEL_SECTOR_OFFSET+128
	mov cx, 128
rd_kern2:	
	call read_floppy_sector
	add bx, 512
	inc si 
	loop rd_kern2
	
	
;检测内存
check_memory:
	xor ebx, ebx 
	mov edx, 0x534d4150
	mov di, ARDS_BUF
	mov ax, 0x600 
	mov es, ax
	mov word [es:ARDS_NR], 0
.e820_mem_get_loop:
	mov eax, 0x0000e820
	mov ecx, 20
	int 0x15
	jc .e820_check_failed
	add di, cx 
	add word [es:ARDS_NR], 1
	cmp ebx, 0
	jnz .e820_mem_get_loop
	jmp init_vbe
.e820_check_failed:
	jmp $

	
	
	
init_vbe:

	


	;检查VBE是否存在
	;缓冲区 0x90000开始
	mov	ax,0x9000	
	mov	es,ax
	mov	di,0
	mov	ax,0x4f00	;检查VBE存在功能，指定ax=0x4f00
	int	0x10
	cmp	ax,0x004f	;ax=0x004f 存在
	jne	screen_default
	
	;检查VBE版本，必须是VBE 2.0及其以上
	mov	ax,[es:di+4]
	cmp	ax,0x0200
	jb	screen_default			; if (ax < 0x0200) goto screen_default

	;获取画面信息， 256字节
	;cx=输入检查的模式
	;[es:di+0x00]	模式属性	bit7是1就能加上0x4000，便于操作
	;[es:di+0x12]	x的分辨率	宽度
	;[es:di+0x14]	y的分辨率	高度
	;[es:di+0x19]	颜色数		8位，16位，24位，32位
	;[es:di+0x1b]	颜色的指定方法 	调色板等
	;[es:di+0x28]	VRAM 地址
	
	mov	cx,VBEMODE	;cx=模式号
	mov	ax,0x4f01	;获取画面模式功能，指定ax=0x4f01
	int	0x10
	cmp	ax,0x004f	;ax=0x004f 指定的这种模式可以使用
	jne	screen_default

	;切换到指定的模式
	mov	BX,VBEMODE+0x4000	;bx=模式号
	mov	ax,0x4f02	;切换模式模式功能，指定ax=0x4f01
	int	0x10
	
	mov ax, 0x610
	mov ds, ax
	
	xor ax, ax
	mov	al,[es:di+0x19]
	mov	[VCOLOR],ax ;保存颜色位数
	mov	ax,[es:di+0x12]
	mov	[XWIDTH],ax		;保存x分辨率 宽度
	mov	ax,[es:di+0x14]
	mov	[YHEIGHT],ax		;保存y分辨率 高度
	mov	eax,[es:di+0x28]
	mov	[VRAM],eax		;保存VRAM地址
	
	JMP	protect_set

	;切换失败切换到默认的模式
screen_default:
	;jmp $
	mov ax, 0x610
	mov ds, ax
	
	mov		al,0x13			;320*200*8彩色
	mov		ah,0x00
	int		0x10
	mov		BYTE [VCOLOR],8
	mov		WORD [XWIDTH],320
	mov		WORD [YHEIGHT],200
	mov		DWORD [VRAM],0x000a0000



protect_set:
	
	mov ax, cs
	mov ds, ax
	
	;close the interruption
	cli
	;load GDTR
	lgdt	[gdt48]
	lidt	[cs:IDTR]

	;enable A20 line
	in		al,0x92
	or		al,0000_0010B
	out		0x92,al
	;set CR0 bit PE
	mov		eax,cr0
	or		eax,1
	mov		cr0,eax
	
	;far jump:to clean the cs
	jmp		dword 0x08:flush

;1kb 对齐，便于gdt地址得整数	
times 1024-($-$$) db 0	
;Global Descriptor Table,GDT
gdt:
	;0:void
	dd		0x00000000
	dd		0x00000000
	;1:4GB(flat-mode) code segment 0
	dd		0x0000ffff
	dd		0x00cf9A00
	;2:4GB(flat-mode) data segment 0
	dd		0x0000ffff
	dd		0x00cf9200
	
	dd		0x00000000
	dd		0x00000000
	
	dd		0x0000ffff
	dd		0x00cffa00
	;0x00cffa000000ffff
	
	dd		0x0000ffff
	dd		0x00cff200
	;DRIVER CODE
	dd		0x00000000
	dd		0x00000000
	
	dd		0x00000000
	dd		0x00000000
	;DRIVER DATA
	dd		0x00000000
	dd		0x00000000
	
	dd		0x00000000
	dd		0x00000000
	
gdt48:
	.size	dw	(8*8-1)
	.offset	 dd gdt

dd		0x00000000
dd		0x00000000	
ADR_IDT	equ		0x00090000
LIMIT_IDT equ	0x000007ff

IDTR:
	.limit		dw	LIMIT_IDT			;IDT的长度
	.base		dd	ADR_IDT			;IDT的物理地址
		
	
char: db 'L'

;si=LBA address, from 0
;cx=sectors
;es:dx=buffer address	
;this function was borrowed from internet
read_floppy_sector:
	push ax 
	push cx 
	push dx 
	push bx 
	
	mov ax, si 
	xor dx, dx 
	mov bx, 18
	
	div bx 
	inc dx 
	mov cl, dl 
	xor dx, dx 
	mov bx, 2
	
	div bx 
	
	mov dh, dl
	xor dl, dl 
	mov ch, al 
	pop bx 
.rp:
	mov al, 0x01
	mov ah, 0x02 
	int 0x13 
	jc .rp 
	pop dx
	pop cx 
	pop ax
	ret 
	
;-------------------------------------------------------------------------------
			   ;功能:读取硬盘n个扇区
rd_disk_m_32:	   
;-------------------------------------------------------------------------------
							 ; eax=LBA扇区号
							 ; ebx=将数据写入的内存地址
							 ; ecx=读入的扇区数
      mov esi,eax	   ; 备份eax
      mov di,cx		   ; 备份扇区数到di
;读写硬盘:
;第1步：设置要读取的扇区数
      mov dx,0x1f2
      mov al,cl
      out dx,al            ;读取的扇区数

      mov eax,esi	   ;恢复ax

;第2步：将LBA地址存入0x1f3 ~ 0x1f6

      ;LBA地址7~0位写入端口0x1f3
      mov dx,0x1f3                       
      out dx,al                          

      ;LBA地址15~8位写入端口0x1f4
      mov cl,8
      shr eax,cl
      mov dx,0x1f4
      out dx,al

      ;LBA地址23~16位写入端口0x1f5
      shr eax,cl
      mov dx,0x1f5
      out dx,al

      shr eax,cl
      and al,0x0f	   ;lba第24~27位
      or al,0xe0	   ; 设置7～4位为1110,表示lba模式
      mov dx,0x1f6
      out dx,al

;第3步：向0x1f7端口写入读命令，0x20 
      mov dx,0x1f7
      mov al,0x20                        
      out dx,al

;;;;;;; 至此,硬盘控制器便从指定的lba地址(eax)处,读出连续的cx个扇区,下面检查硬盘状态,不忙就能把这cx个扇区的数据读出来

;第4步：检测硬盘状态
  .not_ready:		   ;测试0x1f7端口(status寄存器)的的BSY位
      ;同一端口,写时表示写入命令字,读时表示读入硬盘状态
      nop
      in al,dx
      and al,0x88	   ;第4位为1表示硬盘控制器已准备好数据传输,第7位为1表示硬盘忙
      cmp al,0x08
      jnz .not_ready	   ;若未准备好,继续等。

;第5步：从0x1f0端口读数据
      mov ax, di	   ;以下从硬盘端口读数据用insw指令更快捷,不过尽可能多的演示命令使用,
			   ;在此先用这种方法,在后面内容会用到insw和outsw等

      mov dx, 256	   ;di为要读取的扇区数,一个扇区有512字节,每次读入一个字,共需di*512/2次,所以di*256
      mul dx
      mov cx, ax	   
      mov dx, 0x1f0
  .go_on_read:
      in ax,dx		
      mov [es:ebx], ax
      add ebx, 2
			  ; 由于在实模式下偏移地址为16位,所以用bx只会访问到0~FFFFh的偏移。
			  ; loader的栈指针为0x900,bx为指向的数据输出缓冲区,且为16位，
			  ; 超过0xffff后,bx部分会从0开始,所以当要读取的扇区数过大,待写入的地址超过bx的范围时，
			  ; 从硬盘上读出的数据会把0x0000~0xffff的覆盖，
			  ; 造成栈被破坏,所以ret返回时,返回地址被破坏了,已经不是之前正确的地址,
			  ; 故程序出会错,不知道会跑到哪里去。
			  ; 所以改为ebx代替bx指向缓冲区,这样生成的机器码前面会有0x66和0x67来反转。
			  ; 0X66用于反转默认的操作数大小! 0X67用于反转默认的寻址方式.
			  ; cpu处于16位模式时,会理所当然的认为操作数和寻址都是16位,处于32位模式时,
			  ; 也会认为要执行的指令是32位.
			  ; 当我们在其中任意模式下用了另外模式的寻址方式或操作数大小(姑且认为16位模式用16位字节操作数，
			  ; 32位模式下用32字节的操作数)时,编译器会在指令前帮我们加上0x66或0x67，
			  ; 临时改变当前cpu模式到另外的模式下.
			  ; 假设当前运行在16位模式,遇到0X66时,操作数大小变为32位.
			  ; 假设当前运行在32位模式,遇到0X66时,操作数大小变为16位.
			  ; 假设当前运行在16位模式,遇到0X67时,寻址方式变为32位寻址
			  ; 假设当前运行在32位模式,遇到0X67时,寻址方式变为16位寻址.

      loop .go_on_read
      ret	
	
KERNEL_ENTRY EQU 0x10000

PHY_KERNEL_ENTRY EQU 0x10000
VIR_KERNEL_ENTRY EQU 0x80010000

PAGE_diR_ADDR equ 0x1000
PAGE_TBL_ADDR equ 0x2000
[bits 32]
flush:
	;init all segment registeres
	mov ax, 0x10	;the data 
	mov ds, ax 
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
	mov ss, ax 
	mov esp, stack_top_end
	
	;put 'P'
	mov dword [0xb8000+4], 'P'
	mov dword [0xb8000+5], 0X06
	call step_page
	
	mov dword [0x800b8000+4], 'P'
	mov dword [0x800b8000+5], 0X03
	
	;mov esi, PHY_KERNEL_ENTRY
	;mov edi, VIR_KERNEL_ENTRY
	;mov ecx, KERNEL_SECTORS*512
	;call cpy_kernel
	
	
	
	;jmp inio kernel with protect mode,no interrupt,high memory read and write
	jmp VIR_KERNEL_ENTRY
	
cpy_kernel:
.re_cpy:
	mov al, [esi]
	mov [edi], al
	inc esi 
	inc edi
	loop .re_cpy
	ret	
	
step_page:  
    mov ecx,1024                       ;1024个目录项
    mov ebx,PAGE_diR_ADDR                 ;页目录的物理地址
    xor esi,esi
.clean_pdt:
    mov dword [ebx+esi],0x00000000  ;页目录表项清零 
    add esi,4
    loop .clean_pdt

    mov edi, PAGE_TBL_ADDR
    mov ebx, PAGE_diR_ADDR
    mov dword [ebx], PAGE_TBL_ADDR|0x07
    mov dword [ebx+512*4], PAGE_TBL_ADDR|0x07    
    mov dword [ebx+4092], PAGE_diR_ADDR|0x07
   	
   	mov cx, 1024/4
    mov esi, 0|0x07
    
.set_pt0:
    mov [edi], esi
    add esi, 0x1000
    add edi,4
    loop .set_pt0
   	
	;map vram
    mov eax, [0x6100+6]	;ds = 0x7000 phy 0x6108
	shr eax,22
    shl eax,2
	;;eax=0xe00
    
    mov edx,(PAGE_TBL_ADDR+0x1000)|0x07
    mov [PAGE_diR_ADDR+eax], edx
    
    mov edi, PAGE_TBL_ADDR+0x1000	;edi=0x3000
    mov esi, [0x6100+6]	;esi= 0xe0000000
    or esi, 0x07	;esi= 0xe0000007
    
    mov cx, 1024	;map 4M for vram
    
.set_vram:
    mov dword [edi ], esi
	add edi, 4
	add esi, 0x1000
	loop .set_vram
	
   	mov eax , PAGE_diR_ADDR
    mov cr3,eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
.f:
    ret	
	
stack_top:
	times 256 db 0
stack_top_end equ $-stack_top
;fill it with 1kb
times (4096-($-$$)) db 0
