#ifndef __TEXT_H__
#define __TEXT_H__

#include <stdint.h>

extern uint32_t RED;
extern uint32_t DARK_RED;
extern uint32_t GREEN;
extern uint32_t DARK_GREEN;
extern uint32_t YELLOW;
extern uint32_t DARK_YELLOW;
extern uint32_t BLUE;
extern uint32_t DARK_BLUE;
extern uint32_t MAGENTA;
extern uint32_t DARK_MAGENTA;
extern uint32_t CYAN;
extern uint32_t DARK_CYAN;
extern uint32_t WHITE;
extern uint32_t BLACK;
extern uint32_t GRAY;
extern uint32_t DARK_GRAY;
extern uint32_t BG;
extern uint32_t FG;

extern uint32_t fg_color;
extern uint32_t bg_color;

extern uint8_t *font;
extern uint16_t font_width;
extern uint16_t font_height;
extern int x_pos_pixel;
extern int y_pos_pixel;
extern int border;

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
