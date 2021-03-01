#ifndef __TEXT_H__
#define __TEXT_H__

int x;
int y;
int fg_color;
int bg_color;
int border;

int RED;
int DARK_RED;
int GREEN;
int DARK_GREEN;
int YELLOW;
int DARK_YELLOW;
int BLUE;
int DARK_BLUE;
int MAGENTA;
int DARK_MAGENTA;
int CYAN;
int DARK_CYAN;
int WHITE;
int BLACK;
int GRAY;
int DARK_GRAY;
int BG;
int FG;

int init_text(int bor);
int init_color(int red, int dred, int green, int dgreen, int yellow,
               int dyellow, int blue, int dblue, int magenta, int dmagenta,
               int cyan, int dcyan, int white, int black, int gray, int dgray,
               int bg, int fg);

void put(char c, int x, int y, int fgcol, int bgcol);
void puts(const char *string);

int printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);

int cls();
void scroll_screen_up();
void cursor();

#endif // !__TEXT_H__