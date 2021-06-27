#ifndef __FB_H__
#define __FB_H__

#include <boot/stivale2.h>

extern uint32_t curr_fg_col;
extern uint32_t curr_bg_col;

int init_fb(struct stivale2_struct_tag_framebuffer *framebuffer_info);
void putchar(char c);

#endif
