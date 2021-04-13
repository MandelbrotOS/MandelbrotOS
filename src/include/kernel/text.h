#ifndef __TEXT_H__
#define __TEXT_H__

#include <stdint.h>

uint32_t RED;
uint32_t DARK_RED;
uint32_t GREEN;
uint32_t DARK_GREEN;
uint32_t YELLOW;
uint32_t DARK_YELLOW;
uint32_t BLUE;
uint32_t DARK_BLUE;
uint32_t MAGENTA;
uint32_t DARK_MAGENTA;
uint32_t CYAN;
uint32_t DARK_CYAN;
uint32_t WHITE;
uint32_t BLACK;
uint32_t GRAY;
uint32_t DARK_GRAY;
uint32_t BG;
uint32_t FG;

uint32_t fg_color;
uint32_t bg_color;

int x_pos_pixel;
int y_pos_pixel;
int border;

int init_text(int border_);
int init_color(int red, int dred, int green, int dgreen, int yellow,
    int dyellow, int blue, int dblue, int magenta, int dmagenta, int cyan,
    int dcyan, int white, int black, int gray, int dgray, int bg, int fg);
void putc(char ch, int x, int y, uint32_t foreground_color,
    uint32_t background_color);
void putchar(char string);
void puts(const char *string);
void cls();

#endif // !__TEXT_H__
