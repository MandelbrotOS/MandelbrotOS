#ifndef _KERNEL_VESA_DRIVER
#define _KERNEL_VESA_DRIVER

#include <multiboot.h>
#include <stdint.h>

typedef struct rgb {
  uint8_t r, g, b;
} argb_t __attribute__((packed));

void *fb_addr;

uint32_t fb_pitch;
uint32_t fb_width;
uint32_t fb_height;
uint8_t fb_bpp;

uint32_t rgb_to_color(argb_t *);

int putpixel(int x, int y, int color);
int drawrect(int startx, int starty, int stopx, int stopy, int color);
int drawborder(int startx, int starty, int stopx, int stopy, int thickness,
               int color, int incolor);
void mandelbrot(float left, float top, float xside, float yside, int color);

int init_vbe(multiboot_info_t *mbi);

#endif