#include "vga.h"
#include "../io/io.h"

unsigned char back_buf[SCREEN_HEIGHT][SCREEN_WIDTH];

void screen_init() {
	// configure palette with 8-bit RRRGGGBB color
	ioport_out(PALETTE_MASK, 0xFF);
	ioport_out(PALETTE_WRITE, 0);
	for (unsigned char i = 0; i < 255; i++) {
		ioport_out(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
		ioport_out(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
		ioport_out(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
	}

	// set color 255 = white
	ioport_out(PALETTE_DATA, 0x3F);
	ioport_out(PALETTE_DATA, 0x3F);
	ioport_out(PALETTE_DATA, 0x3F);

	clear_buffer();
}

void plot_pixel(int pos_x, int pos_y, char color) {
	unsigned char* location = (unsigned char*)0xA0000 + 320 * pos_y + pos_x;
	*location = color;
}

void k_put_char(char c, int x, int y, unsigned char color) {
	// todo: reimplement printing characters
}

// Function to print a hexadecimal representation of a byte (scancode) at a given position
/*void print_hex(unsigned int value, int x, int y, char color) {
    char hex_buffer[3];  // Buffer to hold the 2 hex digits plus null-terminator
    int_to_hex(value, hex_buffer);  // Convert the value to hex string
    draw_string("0x", x, y, color);  // Print the '0x' prefix
    draw_string(hex_buffer, x + 16, y, color);  // Print the actual hex value (move 16 pixels for next characters)
}

void int_to_hex(unsigned int value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    buffer[0] = hex_digits[(value >> 4) & 0xF]; // High nibble
    buffer[1] = hex_digits[value & 0xF];        // Low nibble
    buffer[2] = '\0'; // Null-terminate the string
}*/

void clear_buffer() {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			back_buf[y][x] = VGA_BLACK;
		}
	}
}

void swap_buffers() {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			plot_pixel(x, y, back_buf[y][x]);
		}
	}
}
