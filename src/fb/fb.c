#include <boot/stivale2.h>
#include <fb/fb.h>
#include <font.h>

uint32_t *framebuffer;
static uint16_t fb_height, fb_width;

uint32_t curr_fg_col = 0xffffff;
uint32_t curr_bg_col = 0x000000;

static int curr_x = 0, curr_y = 0;

int init_fb(struct stivale2_struct_tag_framebuffer *framebuffer_info) {
  framebuffer = (uint32_t *)framebuffer_info->framebuffer_addr;
  fb_width = framebuffer_info->framebuffer_width;
  fb_height = framebuffer_info->framebuffer_height;
  return 0;
}

void putpixel(int x, int y, uint32_t color) {
  framebuffer[y * fb_width + x] = color;
}

void putnc(int x, int y, char c, uint32_t fgc, uint32_t bgc) {
  for (int ly = 0; ly < FONT_HEIGHT; ly++) {
    for (int lx = 0; lx < FONT_WIDTH; lx++) {
      uint8_t pixel = font_array[(c * FONT_CHAR_DEFINITION) + ly];
      if ((pixel >> lx) & 1)
        framebuffer[x + ((FONT_WIDTH - 1) - lx) + ((y + ly) * fb_width)] = fgc;
      else
        framebuffer[x + ((FONT_WIDTH - 1) - lx) + ((y + ly) * fb_width)] = bgc;
    }
  }
}

void knewline() {
  for (int i = 0; i < fb_width * (fb_height - FONT_HEIGHT); i++)
    framebuffer[i] = (framebuffer + fb_width * FONT_HEIGHT)[i];
}

void putc(char c, uint32_t fgc, uint32_t bgc) {
  switch (c) {
  case '\n':
    if (curr_y + FONT_HEIGHT > fb_height)
      knewline();
    else
      curr_y += FONT_HEIGHT;
    break;
  case '\r':
    curr_x = 0;
    break;
  case '\t':
    putc(' ', fgc, bgc);
    break;
  case '\b':
    if (curr_x - FONT_WIDTH < 0 && curr_y - FONT_HEIGHT < 0)
      ;
    else if (curr_x - FONT_WIDTH < 0) {
      curr_y -= FONT_HEIGHT;
      curr_x = fb_width - (fb_width & FONT_WIDTH) - FONT_WIDTH;
      putc(' ', fgc, bgc);
      curr_x -= FONT_WIDTH;
    } else {
      curr_x -= FONT_WIDTH;
      putc(' ', fgc, bgc);
      curr_x -= FONT_WIDTH;
    }
    break;
  default:
    if (curr_x + FONT_WIDTH > fb_width) {
      curr_x = 0;
      putc('\n', fgc, bgc);
    }
    putnc(curr_x, curr_y, c, fgc, bgc);
    curr_x += FONT_WIDTH;
  }
}

void putchar(char c) { putc(c, curr_fg_col, curr_bg_col); }
