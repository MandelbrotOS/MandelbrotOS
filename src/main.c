#include <acpi/acpi.h>
#include <boot/stivale2.h>
#include <drivers/apic.h>
#include <drivers/kbd.h>
#include <drivers/pcspkr.h>
#include <drivers/pit.h>
#include <drivers/serial.h>
#include <fb/fb.h>
#include <klog.h>
#include <mm/kheap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <pci/pci.h>
#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/irq.h>
#include <sys/isr.h>
#include <tasking/scheduler.h>
#include <tasking/smp.h>

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
  struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
  while (1) {
    if (!current_tag)
      return NULL;
    else if (current_tag->identifier == id)
      return current_tag;
    current_tag = (void *)current_tag->next;
  }
}

// TODO Fix sleep
void k_thread() {
  while (1) {
    serial_print("3  3  3  3  3\r\n");
    for (volatile size_t i = 0; i < 5000000; i++)
      asm volatile("nop");
  }
}

void kernel_main(struct stivale2_struct *bootloader_info) {
  struct stivale2_struct_tag_framebuffer *framebuffer_info =
      (struct stivale2_struct_tag_framebuffer *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
  struct stivale2_struct_tag_memmap *memory_info =
      (struct stivale2_struct_tag_memmap *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);
  struct stivale2_struct_tag_rsdp *rsdp_info =
      (struct stivale2_struct_tag_rsdp *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_RSDP_ID);
  struct stivale2_struct_tag_smp *smp_info =
      (struct stivale2_struct_tag_smp *)stivale2_get_tag(
          bootloader_info, STIVALE2_STRUCT_TAG_SMP_ID);

  init_gdt();
  init_idt();
  init_isr();
  init_irq();

  init_pmm(memory_info);
  init_vmm();

  disable_pic();
  init_lapic();
  lapic_timer_init();

  klog(init_fb(framebuffer_info), "Framebuffer");
  klog(init_serial(), "Serial");
  klog(init_heap(), "Heap");
  klog(init_pcspkr(), "PC speaker");
  klog(init_acpi(rsdp_info), "ACPI");
  klog(init_kbd(), "Keyboard");
  klog(pci_enumerate(), "PCI");
  klog(init_pit(), "PIT");
  klog(init_smp(smp_info), "SMP");

  serial_print("Hello, world!");

  scheduler_init((uintptr_t)k_thread, smp_info);

  while (1)
    ;
}
