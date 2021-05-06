#include <halt.h>
#include <hw.h>
#include <printf.h>
#include <string.h>
#include <acpi/acpi.h>
#include <drivers/pcie.h>
#include <kernel/irq.h>
#include <mm/heap.h>
#include <mm/vmm.h>

#include <lai/core.h>
#include <lai/host.h>
#include <lai/helpers/sci.h>

void lai_init(int revision) {
  // get the fadt, it contains SCI info
  struct acpi_fadt *fadt = (struct acpi_fadt *) acpi_get_table("FACP", 0);
  lai_set_acpi_revision(revision);
  lai_create_namespace();
  // now install a SCI handler
  irq_install_handler((int) fadt->sci_int, acpi_sci_handler);
  lai_enable_acpi(0); // 0 if using the PIC, 1: using the APIC
}

// Plugs for LAI
void laihost_log(int level, const char *msg) {
  if (level == LAI_DEBUG_LOG) {
    printf("\nlai: debug: %s\r\n", msg);
  } else if (level == LAI_WARN_LOG) {
    printf("\nlai: warn: %s\r\n", msg);
  } else {
    printf("\nlai: ?: %s\r\n", msg);
  }
}

__attribute__((__noreturn__)) void laihost_panic(const char *msg) {
  printf("lai: panic: %s\r\nHalting.", msg);
  HALT();
  __builtin_unreachable();
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

void *laihost_map(uintptr_t base, size_t size) {
  (void) size;
  return (void *) (base + PHYS_MEM_OFFSET);
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
  pcie_writeb(seg, bus, slot, fun, offset, val);
}

void laihost_pci_writew(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint16_t val) {
  pcie_writew(seg, bus, slot, fun, offset, val);
}

void laihost_pci_writed(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint32_t val) {
  pcie_writed(seg, bus, slot, fun, offset, val);
}

uint8_t laihost_pci_readb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
  return pcie_readb(seg, bus, slot, fun, offset);
}

uint16_t laihost_pci_readw(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
  return pcie_readw(seg, bus, slot, fun, offset);
}

uint32_t laihost_pci_readd(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
  return pcie_readd(seg, bus, slot, fun, offset);
}

void laihost_sleep(uint64_t ms) {
  for (size_t i = 0; i < ms * 1000; i++) {
    inb(0x80);
  }
}
