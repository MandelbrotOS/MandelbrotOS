#ifndef __PCI_H__
#define __PCI_H__

#include <stdbool.h>
#include <stdint.h>

#define PCI_CONFIG 0xCF8
#define PCI_DATA 0xCFC

struct pci_device {
    // todo
};

// Core
uint32_t pci_cfg_read_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
uint16_t pci_cfg_read_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
uint8_t pci_cfg_read_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);

void pci_cfg_write_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t value);
void pci_cfg_write_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint16_t value);
void pci_cfg_write_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t value);

// Helpers (todo)
struct pci_device pci_new_device(uint8_t class, uint8_t subclass, uint8_t prog_if);
void pci_get_bar(struct pci_device *device, uint8_t bar);

#endif // !__PCI_H__
