[org 0x7c00]

KERNEL_OFFSET equ 0x1000

mov [BOOT_DISK], dl

mov bp, 0x9000
mov sp, bp

%ifdef USE_GRAPHICS
call init_video_graphics
%else
call init_video_text
%endif

call load_kernel

call switch_to_pm

jmp $

%include "boot/utils.inc"
%include "boot/gdt.asm"
%include "boot/video.asm"

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET
	mov dh, 20 ; 20 sectors (10KB) for the kernel
	mov dl, [BOOT_DISK]
	call disk_load
	ret

[bits 16]
switch_to_pm:
	cli
	enable_a20
	lgdt [gdt_descriptor]

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:BEGIN_PM

[bits 16]
disk_load:
	pusha

	push dx

	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02

	int 0x13
	
	pop dx

	popa

	ret

[bits 32]
init_pm:
	mov ax, DATA_SEG	; Now in PM, our segment register values from real mode are meaningless.
				; So, we initialize all registers to point to the data segment offset.

	mov ds, ax		; Data segment register (default offset for data addresses)
	mov ss, ax		; Stack segment register (points to segment holding stack)
	mov es, ax		; General-purpose register
	mov fs, ax		; General-purpose register (e.g. for exception handling chain)
	mov gs, ax		; General-purpose register

	mov ebp, 0x90000	; Update our stack position so it is right at the top of free space. This
	mov esp, ebp		; value is left-shifted by 3 since our code segment granularity is set to 1,
				; thus the stack base is at physical address 0x9000000.

	call BEGIN_PM		; Finally, call a well-known label which will be used at the start of the
				; protected-mode code. This is the entry point for our 32-bit code and we
				; will not return from it.

[bits 32]
BEGIN_PM:
	call KERNEL_OFFSET

	jmp $

BOOT_DISK db 0x00

; todo: partition table (lookup mbr scheme you idiot)

times 510-($-$$) db 0
dw 0xaa55
