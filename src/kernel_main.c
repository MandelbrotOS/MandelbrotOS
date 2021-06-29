#include <boot/stivale2.h>
#include <drivers/kbd.h>
#include <drivers/pcspkr.h>
#include <drivers/pit.h>
#include <fb/fb.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <klog.h>
#include <mm/pmm.h>
#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
  struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
  while (1) {
    if (!current_tag) {
      return NULL;
    } else if (current_tag->identifier == id) {
      return current_tag;
    }
    current_tag = (void *)current_tag->next;
  }
}

void kernel_main(struct stivale2_struct *bootloader_info) {
  init_gdt();
  init_idt();
  init_isr();
  init_irq();
  asm volatile("sti");
  struct stivale2_struct_tag_framebuffer *framebuffer_info =
      (struct stivale2_struct_tag_framebuffer *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
  struct stivale2_struct_tag_memmap *memory_info =
      (struct stivale2_struct_tag_memmap *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

  klog(init_fb(framebuffer_info), "Framebuffer");
  klog(init_pmm(memory_info), "Physical memory management");
  klog(init_pit(), "PIT");
  klog(init_pcspkr(), "PC speaker");
  klog(init_kbd(), "Keyboard");

  printf("Hello, world!\r\n");

  while (1)
    ;
}
