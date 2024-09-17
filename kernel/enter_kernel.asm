global keyboard_handler
global enable_interrupts
global load_idt

[bits 32]
[extern main]
[extern handle_keyboard_interrupt]

jmp main
jmp $

keyboard_handler:
	pushad
	call handle_keyboard_interrupt
	popad
	iret

enable_interrupts:
	sti
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	ret
