#include <font.h>
#include <kernel/text.h>
#include <kernel/vbe.h>
#include <multiboot.h>
#include <stdbool.h>

bool baddraw(int x, int y) {
  if ((uint32_t)x > fb_width || (uint32_t)y > fb_height) {
    return true;
  } else {
    return false;
  }
}

int init_vbe(multiboot_info_t *mbi) {
  fb_addr = (void *)(unsigned long)mbi->framebuffer_addr;
  fb_pitch = (uint32_t)mbi->framebuffer_pitch;
  fb_width = (uint32_t)mbi->framebuffer_width;
  fb_height = (uint32_t)mbi->framebuffer_height;
  fb_bpp = (uint8_t)mbi->framebuffer_bpp;

  return 0;
}

int drawrect(int startx, int starty, int stopx, int stopy, int color) {
  int x, y;

  if (baddraw(startx, starty) || baddraw(stopx, stopy)) {
    return 1;
  }

  for (x = startx; x < stopx; x++) {
    for (y = starty; y < stopy; y++) {
      putpixel(x, y, color);
    }
  }

  return 0;
}

int drawborder(int startx, int starty, int stopx, int stopy, int thickness,
               int color, int incolor) {
  int x, y;

  for (x = startx; x < stopx; x++) {
    for (y = starty; y < stopy; y++) {
      putpixel(x, y, color);
    }
  }
  for (x = startx + thickness; x < stopx - thickness; x++) {
    for (y = starty + thickness; y < stopy - thickness; y++) {
      putpixel(x, y, incolor);
    }
  }

  return 0;
}

int putpixel(int x, int y, int color) {
  if (baddraw(x, y)) {
    return 1;
  }

  if (fb_bpp == 8) {
    multiboot_uint8_t *pixel = fb_addr + fb_pitch * y + x;
    *pixel = color;
  } else if (fb_bpp == 15 || fb_bpp == 16) {
    multiboot_uint16_t *pixel = fb_addr + fb_pitch * y + 2 * x;
    *pixel = color;
  } else if (fb_bpp == 24) {
    multiboot_uint32_t *pixel = fb_addr + fb_pitch * y + 3 * x;
    *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
  } else if (fb_bpp == 32) {
    multiboot_uint32_t *pixel = fb_addr + fb_pitch * y + 4 * x;
    *pixel = color;
  }
  return 0;
}

void mandelbrot(float left, float top, float xside, float yside, int color) {
  float xscale, yscale, zx, zy, cx, tempx, cy;
  int x, y, i, j;
  int maxx, maxy, count;

  cls();

  // getting maximum value of x-axis of screen
  maxx = fb_width;

  // getting maximum value of y-axis of screen
  maxy = fb_height;

  // setting up the xscale and yscale
  xscale = xside / maxx;
  yscale = yside / maxy;

  // scanning every point in that rectangular area.
  // Each point represents a Complex number (x + yi).
  // Iterate that complex number
  for (y = 1; y <= maxy - 1; y++) {
    for (x = 1; x <= maxx - 1; x++) {
      // c_real
      cx = x * xscale + left;

      // c_imaginary
      cy = y * yscale + top;

      // z_real
      zx = 0;

      // z_imaginary
      zy = 0;
      count = 0;

      // Calculate whether c(c_real + c_imaginary) belongs
      // to the Mandelbrot set or not and draw a pixel
      // at coordinates (x, y) accordingly
      // If you reach the Maximum number of iterations
      // and If the distance from the origin is
      // greater than 2 exit the loop
      while ((zx * zx + zy * zy < 4) && (count < 30)) {
        // Calculate Mandelbrot function
        // z = z*z + c where z is a complex number

        // tempx = z_real*_real - z_imaginary*z_imaginary + c_real
        tempx = zx * zx - zy * zy + cx;

        // 2*z_real*z_imaginary + c_imaginary
        zy = 2 * zx * zy + cy;

        // Updating z_real = tempx
        zx = tempx;

        // Increment count
        count = count + 1;
      }

      // To display the created fractal
      putpixel(x, y, (count * 8) + color);
    }
  }
}