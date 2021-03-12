#include <boot/stivale2.h>
#include <font.h>
#include <kernel/fb.h>
#include <kernel/text.h>
#include <printf.h>
#include <stdint.h>
#include <string.h>
#include <kernel/idt.h>

int kernel_main(struct stivale2_struct *bootloader_info) {

  // First get the framebuffer
  struct stivale2_tag *tag;
  if ((tag = stivale2_get_tag(bootloader_info,
                              STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID))) {
    struct stivale2_struct_tag_framebuffer *framebuffer_info =
        (struct stivale2_struct_tag_framebuffer *)tag;
    init_fb((void *)framebuffer_info->framebuffer_addr,
            framebuffer_info->framebuffer_width,
            framebuffer_info->framebuffer_height);
    init_color(0xff0000, 0x990000, 0x00ff00, 0x009900, 0xffff00, 0x999900,
               0x0000ff, 0x000099, 0xff00ff, 0x990099, 0x00ffff, 0x009999,
               0xffffff, 0x000000, 0x999999, 0x444444, 0x161925, 0xdedede);
    init_text(5);
  } else {
    return 1;
  }

  init_idt();

  //printf("%s%f", "Hello world!\r\n", 3.14159);

  return 0;
}