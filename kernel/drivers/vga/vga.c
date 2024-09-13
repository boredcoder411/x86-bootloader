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
