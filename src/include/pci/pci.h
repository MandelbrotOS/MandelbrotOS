#ifndef __PCI_H__
#define __PCI_H__

#include <stddef.h>
#include <stdint.h>
#include <acpi/tables.h>

extern mcfg_t *mcfg;

typedef struct pci_edev {
  uint16_t vendor_id;
  uint16_t device_id;
  uint16_t command;
  uint16_t status;
  uint8_t revision_id;
  uint8_t prog_if;
  uint8_t subclass;
  uint8_t class;
  uint8_t cache_lsize;
  uint8_t latency_timer;
  uint8_t header_type;
  uint8_t bist;
} pci_device_t;

extern const char *pci_device_classes[];

const char *pci_get_subclass_name(uint8_t class, uint8_t subclass);
const char *pci_get_vendor_name(uint16_t vendor_id);
const char *pci_get_device_name(uint16_t vendor_id, uint16_t device_id);
int pci_enumerate();

uint8_t pci_cfg_read_byte(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, uint16_t offset);
uint16_t pci_cfg_read_word(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, uint16_t offset);
uint32_t pci_cfg_read_dword(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, uint16_t offset);

void pci_cfg_write_byte(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, uint16_t offset, uint8_t value);
void pci_cfg_write_word(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, uint16_t offset, uint16_t value);
void pci_cfg_write_dword(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, uint16_t offset, uint32_t value);

enum PCI_BAR_BITNESS {
  PCI_BAR_BITNESS_32 = 0x00,
  PCI_BAR_BITNESS_16 = 0x01,
  PCI_BAR_BITNESS_64 = 0x02
};

enum PCI_BAR_KIND {
  PCI_BAR_KIND_MMIO = 0,
  PCI_BAR_KIND_IO = 1
};

struct pci_bar {
  uint64_t base;
  size_t size;
  enum PCI_BAR_KIND kind;
};

int pci_get_bar(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function, int bar, struct pci_bar *bar_info);

#endif
