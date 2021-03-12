#include <kernel/fb.h>
#include <stdint.h>

void init_fb(void *framebuffer_, uint32_t fb_width, uint32_t fb_height) {
  framebuffer = framebuffer_;
  framebuffer_width = fb_width;
  framebuffer_height = fb_height;
}

void putpixel(int x, int y, uint32_t color) {
  framebuffer[y * framebuffer_width + x] = color;
}

void drawrect(int startx, int starty, int stopx, int stopy, uint32_t color) {
  for (int dx = startx; dx < stopx; dx++) {
    for (int dy = starty; dy < stopy; dy++) {
      framebuffer[dy * framebuffer_width + dx] = color;
    }
  }
}