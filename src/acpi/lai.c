#include <hw.h>
#include <printf.h>
#include <string.h>
#include <acpi/acpi.h>
#include <drivers/pci.h>
#include <mm/heap.h>
#include <mm/vmm.h>

#include <lai/core.h>
#include <lai/host.h>

void lai_init(int revision) {
  lai_set_acpi_revision(revision);
}

void laihost_log(int level, const char *msg) {
  if (level == LAI_DEBUG_LOG) {
    printf("\nlai: debug: %s\n", msg);
  } else if (level == LAI_WARN_LOG) {
    printf("\nlai: warn: %s\n", msg);
  } else {
    printf("\nlai: ?: %s\n", msg);
  }
}

void laihost_panic(const char *msg) {
  printf("lai: panic: %s\nHalting.", msg);
  asm volatile("1:\n\tcli\n\thlt\n\tjmp 1b");
}

void *laihost_malloc(size_t size) {
  return kmalloc(size);
}

void *laihost_realloc(void *old, size_t newsize, size_t oldsize) {
  void *new = kmalloc(newsize);
  memcpy(new, old, oldsize);
  kfree(old);
  return new;
}

void laihost_free(void *base, size_t size) {
  (void) size;
  kfree(base);
}

void *laihost_map(size_t base, size_t size) {
  (void) size;
  return (void *) (base + 0xffff800000000000);
}

void laihost_unmap(void *pointer, size_t count) {
  (void) pointer;
  (void) count;
}

void *laihost_scan(const char *sig, size_t index) {
  return acpi_get_table(sig, index);
}

void laihost_outb(uint16_t port, uint8_t val) {
  outb(port, val);
}

void laihost_outw(uint16_t port, uint16_t val) {
  outw(port, val);
}

void laihost_outd(uint16_t port, uint32_t val) {
  outd(port, val);
}

uint8_t laihost_inb(uint16_t port) {
  return inb(port);
}

uint16_t laihost_inw(uint16_t port) {
  return inw(port);
}

uint32_t laihost_ind(uint16_t port) {
  return ind(port);
}

void laihost_pci_writeb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint8_t val) {
  uintptr_t segment_base = (uintptr_t) acpi_mcfg_get_seg_addr(seg);
  *((volatile uint8_t *) segment_base + (((bus * 256) + (slot * 8) + fun) * 4096) + offset) = val;
}

void laihost_pci_writew(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint16_t val) {
  uintptr_t segment_base = (uintptr_t) acpi_mcfg_get_seg_addr(seg);
  *((volatile uint16_t *) (segment_base + (((bus * 256) + (slot * 8) + fun) * 4096) + offset)) = val;
}

void laihost_pci_writed(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint32_t val) {
  uintptr_t segment_base = (uintptr_t) acpi_mcfg_get_seg_addr(seg);
  *((volatile uint32_t *) (segment_base + (((bus * 256) + (slot * 8) + fun) * 4096) + offset)) = val;
}

uint8_t laihost_pci_readb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
  uintptr_t segment_base = (uintptr_t) acpi_mcfg_get_seg_addr(seg);
  return *((volatile uint8_t *) segment_base + (((bus * 256) + (slot * 8) + fun) * 4096) + offset);
}

uint16_t laihost_pci_readw(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
  uintptr_t segment_base = (uintptr_t) acpi_mcfg_get_seg_addr(seg);
  return *((volatile uint16_t *) segment_base + (((bus * 256) + (slot * 8) + fun) * 4096) + offset);
}

uint32_t laihost_pci_readd(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
  uintptr_t segment_base = (uintptr_t) acpi_mcfg_get_seg_addr(seg);
  return *((volatile uint32_t *) segment_base + (((bus * 256) + (slot * 8) + fun) * 4096) + offset);
}

void laihost_sleep(uint64_t ms) {
  for (size_t i = 0; i < ms * 1000; i++) {
    inb(0x80);
  }
}

void laihost_handle_amldebug(lai_variable_t *var) {
  (void) var;
  return;
}