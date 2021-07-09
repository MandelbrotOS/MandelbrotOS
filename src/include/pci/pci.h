#ifndef __PCI_H__
#define __PCI_H__
#include <acpi/tables.h>
#include <stdint.h>

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

#endif
