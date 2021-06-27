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

  klog(init_fb(framebuffer_info), "Framebuffer");
  klog(init_pit(), "PIT");
  klog(init_pcspkr(), "PC speaker");
  klog(init_kbd(), "Keyboard");

  pcspkr_beep(400);

  printf("\r\nKLOG TEST\r\n\r\n");
  klog(0, "Success colours");
  klog(1, "Failure colours");
  klog(2, "Warning colours");
  klog(3, "Info colours");

  while (1) {
    char echo[100] = "";
    printf("$ ");
    getline(echo, 100);
    printf("\nYou typed: %s\r\n", echo);
  }
}
