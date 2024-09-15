#include <stdbool.h>
#include "drivers/vga/vga.h"

#define IDT_SIZE 256
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define IDT_INTERRUPT_GATE 0x8e

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA   (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA   (PIC2+1)
#define PIC_EOI     0x20

#define ICW1 0x11
#define ICW4 0x01

#include "keyboard_map.h"

extern void enable_interrupts();
extern void keyboard_handler();
//extern char ioport_in(unsigned short port);
//extern void ioport_out(unsigned short port, unsigned char data);
extern void load_idt(unsigned int* idt_address);

void ioport_out(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

char ioport_in(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

struct IDT_pointer {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct IDT_entry {
	unsigned short offset_lowerbits;
	unsigned short selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short offset_higherbits;
} __attribute__((packed));

struct IDT_entry IDT[IDT_SIZE];

void init_idt() {
	unsigned int offset = (unsigned int)keyboard_handler;
	IDT[0x21].offset_lowerbits = offset & 0x0000ffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = IDT_INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (offset & 0xffff0000) >> 16;

	// ICW1
	ioport_out(PIC1_COMMAND, ICW1);
	ioport_out(PIC2_COMMAND, ICW1);

	// ICW2, irq 0 to 7 is mapped to 0x20 to 0x27, irq 8 to F is mapped to 28 to 2F
	ioport_out(PIC1_DATA, 0x20);
	ioport_out(PIC2_DATA, 0x28);

	// ICW3, connect master pic with slave pic
	ioport_out(PIC1_DATA, 0x4);
	ioport_out(PIC2_DATA, 0x2);

	// ICW4, set x86 mode
	ioport_out(PIC1_DATA, 1);
	ioport_out(PIC2_DATA, 1);

	// clear the mask register
	ioport_out(PIC1_DATA, 0);
	ioport_out(PIC2_DATA, 0);

	struct IDT_pointer idt_ptr;
	idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
	idt_ptr.base = (unsigned int)&IDT;
	// kernel crashes at load_idt, proven by the fact that "here6" is not printed, only "here" to "here5"
	load_idt((unsigned int*) &idt_ptr);
}

void init_keyboard() {
	ioport_out(PIC1_DATA, 0xfd);
}

void handle_keyboard_interrupt() {
	ioport_out(PIC1_COMMAND, 0x20);  // End of interrupt
	unsigned char status = ioport_in(KEYBOARD_STATUS_PORT);
	if (status & 0x01) {  // Check if data is available
	unsigned char keycode = ioport_in(KEYBOARD_DATA_PORT);
		if (keycode < 128) {  // Valid keycode range
			char key = kbdus[keycode];
			k_put_char(key, 0, 0, make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));  // Print the character
			print_hex(keycode, 0, 8, make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
		}
	}
	return;
}

void main() {
	// Define a color with light cyan foreground and black background
	char color = make_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);

	// Print a string with newline support
	k_put_string("Hello, World!\nWelcome to VGA text mode.", 0, 5, color);
	
	init_idt();
	init_keyboard();
	enable_interrupts();

	while (true) {}  // Infinite loop to prevent the program from exiting
}

