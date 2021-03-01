#ifndef __TEXT_H__
#define __TEXT_H__

extern int x;
extern int y;
extern int fg_color;
extern int bg_color;
extern int border;

extern int RED;
extern int DARK_RED;
extern int GREEN;
extern int DARK_GREEN;
extern int YELLOW;
extern int DARK_YELLOW;
extern int BLUE;
extern int DARK_BLUE;
extern int MAGENTA;
extern int DARK_MAGENTA;
extern int CYAN;
extern int DARK_CYAN;
extern int WHITE;
extern int BLACK;
extern int GRAY;
extern int DARK_GRAY;
extern int BG;
extern int FG;

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
