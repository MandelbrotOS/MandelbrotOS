#ifndef __FB_H__
#define __FB_H__

#include <boot/stivale2.h>

int init_fb(struct stivale2_struct_tag_framebuffer *framebuffer_info);
void putchar(char c);

#endif
