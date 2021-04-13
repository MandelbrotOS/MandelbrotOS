#ifndef __VBE_H__
#define __VBE_H__

#include <stdint.h>

uint32_t *framebuffer;

uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint8_t framebuffer_bpp;

int init_fb(void *framebuffer_, uint32_t fb_width, uint32_t fb_height);
void putpixel(int x, int y, uint32_t color);
void drawrect(int startx, int starty, int stopx, int stopy, uint32_t color);

#endif // !__VBE_H__
