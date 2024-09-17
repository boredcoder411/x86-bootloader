#include <stdint.h>  // For standard integer types

#define COLOR(_r, _g, _b) ((unsigned char)((((_r) & 0x7) << 5) | (((_g) & 0x7) << 2) | (((_b) & 0x3) << 0)))

#define VGA_BLACK COLOR(0, 0, 0)
#define VGA_WHITE COLOR(7, 7, 3)
#define VGA_RED COLOR(7, 0, 0)
#define VGA_GREEN COLOR(0, 7, 0)
#define VGA_BLUE COLOR(0, 0, 3)
#define VGA_YELLOW COLOR(7, 7, 0)
#define VGA_MAGENTA COLOR(7, 0, 3)
#define VGA_CYAN COLOR(0, 7, 3)

#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

// Function prototypes
void k_put_char(char c, int x, int y, unsigned char color);
void k_put_string(const char* str, int start_x, int start_y, unsigned char color);
void k_clear_screen(char color);
void plot_pixel(int x, int y, char color);
void screen_init();
