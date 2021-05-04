#include <stdint.h>
#include <drivers/pci.h>
#include <hw.h>

uint32_t pci_cfg_read_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
  uint32_t address = (uint32_t) 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | ((uint32_t) offset);
  outd(PCI_CONFIG, address);
  return ind(PCI_DATA + (offset & 3));
}

uint16_t pci_cfg_read_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
  uint32_t address = (uint32_t) 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | ((uint32_t) offset);
  outd(PCI_CONFIG, address);
  return inw(PCI_DATA + (offset & 3));
}

uint8_t pci_cfg_read_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
  uint32_t address = (uint32_t) 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | ((uint32_t) offset);
  outd(PCI_CONFIG, address);
  return inb(PCI_DATA + (offset & 3));
}

void pci_cfg_write_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t value) {
  uint32_t address = (uint32_t) 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | ((uint32_t) offset);
  outd(PCI_CONFIG, address);
  outd(PCI_DATA + (offset & 3), value);
}

void pci_cfg_write_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint16_t value) {
  uint32_t address = (uint32_t) 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | ((uint32_t) offset);
  outd(PCI_CONFIG, address);
  outw(PCI_DATA + (offset & 3), value);
}

void pci_cfg_write_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t value) {
  uint32_t address = (uint32_t) 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | ((uint32_t) offset);
  outd(PCI_CONFIG, address);
  outb(PCI_DATA + (offset & 3), value);
}
