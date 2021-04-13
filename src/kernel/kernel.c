#include <boot/stivale2.h>
#include <drivers/ata.h>
#include <drivers/pit.h>
#include <font.h>
#include <hw.h>
#include <kernel/fb.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/text.h>
#include <mm/heap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <printf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Kernel entry
int kernel_main(struct stivale2_struct_t *bootloader_info)
{
  struct stivale2_struct_tag_framebuffer_t *framebuffer_info
      = (struct stivale2_struct_tag_framebuffer_t *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

  struct stivale2_struct_tag_memmap_t *memory_map
      = (struct stivale2_struct_tag_memmap_t *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

  struct stivale2_struct_tag_smp_t *smp_info
      = (struct stivale2_struct_tag_smp_t *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_SMP_ID);

  if (framebuffer_info && memory_map) {
    init_fb((void *)framebuffer_info->framebuffer_addr,
        framebuffer_info->framebuffer_width,
        framebuffer_info->framebuffer_height);

    init_color(0xff0000, 0x990000, 0x00ff00, 0x009900, 0xffff00, 0x999900,
        0x0000ff, 0x000099, 0xff00ff, 0x990099, 0x00ffff, 0x009999, 0xffffff,
        0x000000, 0x999999, 0x444444, 0x161925, 0xdedede);
    init_text(5);

    init_idt();

    init_isr();

    init_irq();

    __asm__ volatile("sti");

    init_pit();

    init_pmm(memory_map->memmap, memory_map->entries);

    init_vmm();

    init_heap(pmalloc(32), 32 * PAGE_SIZE);

  } else {
    return 1;
  }

  return 0;
}
