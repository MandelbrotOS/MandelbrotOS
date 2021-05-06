#include <stddef.h>
#include <stdint.h>
#include <halt.h>
#include <printf.h>
#include <acpi/acpi.h>
#include <mm/vmm.h>

static struct acpi_mcfg *mcfg;

// A segment is a collection of PCI buses
static void *pcie_get_device_addr(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
  size_t entries = (mcfg->header.length - sizeof(struct acpi_mcfg)) / sizeof(struct acpi_mcfg_entry);
  for (size_t i = 0; i < entries; i++) {
    if (
      mcfg->entries[i].segment_number == segment
      && mcfg->entries[i].start_pci_bus >= bus
      && mcfg->entries[i].end_pci_bus <= bus
    ) {
      return (void *) (
        mcfg->entries[i].base
        + (((bus - mcfg->entries[i].start_pci_bus) << 20) | (slot << 15) | (function << 12))
        + offset
        + PHYS_MEM_OFFSET
      );
    }
  }
  return NULL;
}

void init_pcie() {
  mcfg = acpi_get_table("MCFG", 0);
  if (!mcfg) {
    printf("pcie: panic: MCFG table not found.\r\nIf using QEMU, did you use the -machine q35 flag?\r\nHalting.");
    HALT();
  }
}

uint8_t pcie_readb(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
  return *((uint8_t *) pcie_get_device_addr(segment, bus, slot, function, offset));
}

uint16_t pcie_readw(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
  return *((uint16_t *) pcie_get_device_addr(segment, bus, slot, function, offset));
}

uint32_t pcie_readd(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
  return *((uint32_t *) pcie_get_device_addr(segment, bus, slot, function, offset));
}

void pcie_writeb(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint8_t value) {
  *((uint8_t *) pcie_get_device_addr(segment, bus, slot, function, offset)) = value;
}

void pcie_writew(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint16_t value) {
  *((uint16_t *) pcie_get_device_addr(segment, bus, slot, function, offset)) = value;
}

void pcie_writed(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint32_t value) {
  *((uint32_t *) pcie_get_device_addr(segment, bus, slot, function, offset)) = value;
}
