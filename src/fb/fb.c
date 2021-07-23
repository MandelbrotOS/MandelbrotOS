#include <boot/stivale2.h>
#include <fb/fb.h>
#include <font.h>
#include <lock.h>
#include <stddef.h>
#include <stdint.h>

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

void set_bg(uint32_t bg_col) {
  MAKE_LOCK(set_bg_lock);
  curr_bg_col = bg_col;
  UNLOCK(set_bg_lock);
}

void set_fg(uint32_t fg_col) {
  MAKE_LOCK(set_fg_lock);
  curr_fg_col = fg_col;
  UNLOCK(set_fg_lock);
}

void putpixel(int x, int y, uint32_t color) {
  MAKE_LOCK(putpixel_lock);
  framebuffer[y * fb_width + x] = color;
  UNLOCK(putpixel_lock);
}

void putnc(int x, int y, char c, uint32_t fgc, uint32_t bgc) {
  MAKE_LOCK(putnc_lock);
  for (int ly = 0; ly < FONT_HEIGHT; ly++) {
    for (int lx = 0; lx < FONT_WIDTH; lx++) {
      uint8_t pixel = font_array[(c * FONT_CHAR_DEFINITION) + ly];
      if ((pixel >> lx) & 1)
        framebuffer[x + ((FONT_WIDTH - 1) - lx) + ((y + ly) * fb_width)] = fgc;
      else
        framebuffer[x + ((FONT_WIDTH - 1) - lx) + ((y + ly) * fb_width)] = bgc;
    }
  }
  UNLOCK(putnc_lock);
}

void knewline() {
  MAKE_LOCK(newline_lock);
  for (size_t i = 0; i < (size_t)(fb_width * fb_height); i++)
    framebuffer[i] = (framebuffer + fb_width * FONT_HEIGHT)[i];
  for (size_t i = 0; i < (size_t)(fb_width * fb_height); i++)
    framebuffer[i] = (framebuffer + fb_width * FONT_HEIGHT)[i];
  for (size_t i = 0; i < (size_t)(fb_height * (FONT_HEIGHT + 2)); i++)
    (framebuffer + (fb_width * fb_height) - (fb_width * FONT_HEIGHT))[i] = curr_bg_col;
  curr_y -= FONT_HEIGHT;
  curr_x = 0;
  UNLOCK(newline_lock);
}

void putc(char c, uint32_t fgc, uint32_t bgc) {
  MAKE_LOCK(putc_lock);
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
  case '\b':
    if (curr_x - FONT_WIDTH < 0 && curr_y - FONT_HEIGHT < 0)
      ;
    else if (curr_x - FONT_WIDTH < 0) {
      curr_y -= FONT_HEIGHT;
      curr_x = fb_width - (fb_width & FONT_WIDTH) - FONT_WIDTH;
      putnc(curr_x, curr_y, ' ', fgc, bgc);
    } else {
      curr_x -= FONT_WIDTH;
      putnc(curr_x, curr_y, ' ', fgc, bgc);
    }
    break;
  default:
    if (curr_x + FONT_WIDTH > fb_width) {
      curr_x = 0;
      if (curr_y + FONT_HEIGHT > fb_height)
        knewline();
      else
        curr_y += FONT_HEIGHT;
    }
    putnc(curr_x, curr_y, c, fgc, bgc);
    curr_x += FONT_WIDTH;
  }
  UNLOCK(putc_lock);
}

void putchar(char c) { putc(c, curr_fg_col, curr_bg_col); }
