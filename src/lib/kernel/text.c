#include <font.h>
#include <kernel/fb.h>
#include <kernel/text.h>
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

int init_color(int red, int dred, int green, int dgreen, int yellow,
               int dyellow, int blue, int dblue, int magenta, int dmagenta,
               int cyan, int dcyan, int white, int black, int gray, int dgray,
               int bg, int fg) {
  RED = red;
  DARK_RED = dred;
  GREEN = green;
  DARK_GREEN = dgreen;
  YELLOW = yellow;
  DARK_YELLOW = dyellow;
  BLUE = blue;
  DARK_BLUE = dblue;
  MAGENTA = magenta;
  DARK_MAGENTA = dmagenta;
  CYAN = cyan;
  DARK_CYAN = dcyan;
  WHITE = white;
  BLACK = black;
  GRAY = gray;
  DARK_GRAY = dgray;
  BG = bg;
  FG = fg;

  return 0;
}

int init_text(int border_) {
  fg_color = FG;
  bg_color = BG;

  x_pos_pixel = border_;
  y_pos_pixel = border_;

  border = border_;

  cls();

  return 0;
}

void cls() {
  x_pos_pixel = border;
  y_pos_pixel = border;
  for (int i = 0; (uint32_t)i < framebuffer_width * framebuffer_height; i++) {
    framebuffer[i] = bg_color;
  }
}

void backspace() {
  if (x_pos_pixel >= GLYPH_WIDTH + 1) {
    putc(' ', x_pos_pixel, y_pos_pixel, bg_color, bg_color);
    x_pos_pixel -= GLYPH_WIDTH + 1;
    putc(' ', x_pos_pixel, y_pos_pixel, bg_color, bg_color);
  } else {
    if (y_pos_pixel >= 9) {
      y_pos_pixel -= (GLYPH_WIDTH + 1);
      x_pos_pixel = (int)framebuffer_width - border - (GLYPH_WIDTH);
      putc(' ', x_pos_pixel, y_pos_pixel, bg_color, bg_color);
    }
  }
}

void scroll_screen_up() {
  for (uint32_t i = 1; i <= framebuffer_width * framebuffer_height; i++) {
    framebuffer[i] = framebuffer[i + (framebuffer_width * (GLYPH_WIDTH + 1))];
  }
  drawrect(0, 0, framebuffer_width, border, bg_color);
  drawrect(
      0, (int)framebuffer_height - (border + (GLYPH_HEIGHT + 1) + (border - 1)),
      framebuffer_width, framebuffer_height, bg_color);
}

void putc(char ch, int x, int y, uint32_t foreground_color,
          uint32_t background_color) {
  uint8_t *bitmap = (uint8_t *)char_font[ch % 128];
  for (int lx = 0; lx < GLYPH_WIDTH; lx++) {
    for (int ly = 0; ly < GLYPH_HEIGHT; ly++) {
      uint8_t row = bitmap[ly];
      if ((row >> lx) & 1)
        putpixel(x + lx, y + ly, foreground_color);
      else
        putpixel(x + lx, y + ly, background_color);
    }
  }
}

void putchar(char string) {
  if ((uint32_t)x_pos_pixel > framebuffer_width - border) {
    x_pos_pixel = border;
    y_pos_pixel += GLYPH_HEIGHT + 1;
  }

  if ((uint32_t)y_pos_pixel >
      framebuffer_height - border - (GLYPH_HEIGHT + 1)) {
    y_pos_pixel -= (GLYPH_HEIGHT + 1);
    scroll_screen_up();
  }

  if (string == '\r') {
    x_pos_pixel = border;
  } else if (string == '\n') {
    y_pos_pixel += GLYPH_HEIGHT + 1;
  } else if (string == '\t') {
    for (int t = 0; t != 2; t++) {
      putc(' ', x_pos_pixel, y_pos_pixel, bg_color, bg_color);
      x_pos_pixel += GLYPH_WIDTH + 1;
    }
  } else if (string == '\b') {
    backspace();
  } else {
    putc(string, x_pos_pixel, y_pos_pixel, fg_color, bg_color);
    x_pos_pixel += GLYPH_WIDTH + 1;
  }
}

void puts(const char *string) {
  while (*string != 0) {
    putchar(*string++);
  }
}
