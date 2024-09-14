#include "vga.h"

// Function to output a single character at a specific position
void k_put_char(char c, int x, int y, char color) {
    char* video_memory = (char*) 0xb8000;
    int offset = (y * 80 + x) * 2;  // Calculate the position in the VGA buffer
    video_memory[offset] = c;
    video_memory[offset + 1] = color;
}

// Function to output a string, supporting newlines
void k_put_string(const char* str, int start_x, int start_y, char color) {
    int x = start_x;
    int y = start_y;

    while (*str != '\0') {
        if (*str == '\n') {
            x = 0;  // Move cursor to the start of the next line
            y++;    // Move down one row
            if (y >= 25) {
                y = 0; // Wrap around to the top if we exceed the screen height
            }
        } else {
            k_put_char(*str, x, y, color); // Print character at current position
            x++;
            if (x >= 80) {
                x = 0;  // Move cursor to the start of the next line
                y++;    // Move down one row
                if (y >= 25) {
                    y = 0; // Wrap around to the top if we exceed the screen height
                }
            }
        }
        str++;  // Move to the next character
    }
}

void k_clear_screen(char color) {
    for (int y = 0; y < 25; y++) {
	for (int x = 0; x < 80; x++) {
	    k_put_char('\0', x, y, color);
	}
    }
}

void int_to_hex(unsigned int value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    buffer[0] = hex_digits[(value >> 4) & 0xF]; // High nibble
    buffer[1] = hex_digits[value & 0xF];        // Low nibble
    buffer[2] = '\0'; // Null-terminate the string
}

// Function to print a hexadecimal representation of a byte (scancode) at a given position
void print_hex(unsigned int value, int x, int y, char color) {
    char hex_buffer[3];  // Buffer to hold the 2 hex digits plus null-terminator
    int_to_hex(value, hex_buffer);  // Convert the value to hex string
    k_put_string("0x", x, y, color);  // Print the '0x' prefix
    k_put_string(hex_buffer, x + 2, y, color);  // Print the actual hex value
}
