#ifndef __TEXT_H__
#define __TEXT_H__

#include <stdint.h>

static uint32_t RED;
static uint32_t DARK_RED;
static uint32_t GREEN;
static uint32_t DARK_GREEN;
static uint32_t YELLOW;
static uint32_t DARK_YELLOW;
static uint32_t BLUE;
static uint32_t DARK_BLUE;
static uint32_t MAGENTA;
static uint32_t DARK_MAGENTA;
static uint32_t CYAN;
static uint32_t DARK_CYAN;
static uint32_t WHITE;
static uint32_t BLACK;
static uint32_t GRAY;
static uint32_t DARK_GRAY;
static uint32_t BG;
static uint32_t FG;

uint32_t fg_color;
uint32_t bg_color;

uint8_t *font;
int x_pos_pixel;
int y_pos_pixel;
int border;

int init_text(int border_);
int init_color(int red, int dred, int green, int dgreen, int yellow,
               int dyellow, int blue, int dblue, int magenta, int dmagenta,
               int cyan, int dcyan, int white, int black, int gray, int dgray,
               int bg, int fg);
void putc(char ch, int x, int y, uint32_t foreground_color,
          uint32_t background_color);
void putchar(char string);
void puts(const char *string);
void cls();

#endif // !__TEXT_H__
