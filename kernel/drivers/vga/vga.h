#ifndef VGA_H
#define VGA_H

#include <stdint.h>  // For standard integer types

// Define VGA colors
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

// Inline function to create a color byte from foreground and background colors
static inline char make_color(vga_color fg, vga_color bg) {
    return (bg << 4) | (fg & 0x0F);
}

// Function prototypes
void k_put_char(char c, int x, int y, char color);
void k_put_string(const char* str, int start_x, int start_y, char color);
void k_clear_screen(char color);

#endif  // VGA_H
