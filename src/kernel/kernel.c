#include <boot/stivale2.h>
#include <drivers/ata.h>
#include <font.h>
#include <kernel/fb.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/text.h>
#include <mm/pmm.h>
#include <printf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Kernel entry
int kernel_main(struct stivale2_struct *bootloader_info) {
  // First get the framebuffer
  struct stivale2_tag *tag;
  struct stivale2_tag *mem;

  struct stivale2_struct_tag_framebuffer *framebuffer_info =
    (struct stivale2_struct_tag_framebuffer *)
      stivale2_get_tag(bootloader_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
  struct stivale2_struct_tag_memmap *memory_map =
    (struct stivale2_struct_tag_memmap *)
      stivale2_get_tag(bootloader_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);
  if (framebuffer_info && memory_map) {
    init_fb((void *)framebuffer_info->framebuffer_addr,
            framebuffer_info->framebuffer_width,
            framebuffer_info->framebuffer_height);

    init_color(0xff0000, 0x990000, 0x00ff00, 0x009900, 0xffff00, 0x999900,
               0x0000ff, 0x000099, 0xff00ff, 0x990099, 0x00ffff, 0x009999,
               0xffffff, 0x000000, 0x999999, 0x444444, 0x161925, 0xdedede);
    init_text(5);

    init_idt();

    pmm_init(memory_map->memmap, memory_map->entries);
  } else {
    return 1;
  }

  return 0;
}
