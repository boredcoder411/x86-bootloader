;
; A simple routine for entering the kernel at the function `main`.
;

global keyboard_handler
global enable_interrupts
global load_idt

[bits 32]     ; We're in protected mode, so use 32-bit instructios.
[extern main] ; Expect `main` to be defined in an assembly file assembled alongside this one.
[extern handle_keyboard_interrupt]

jmp main      ; Enter the main function.
jmp $         ; When control returns from the kernel, hang.

keyboard_handler:
	pushad
	call handle_keyboard_interrupt
	popad
	iret

enable_interrupts:
	sti
	iret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	iret
