#ifndef __PCI_H__
#define __PCI_H__
#include <stdint.h>

typedef struct pci_device {
  uint8_t bus, device, function;
  uint8_t class, subclass;
  uint16_t device_id, vendor_id;
} pci_t;

void pci_legacy_enumerate();
void pci_enumerate();

#endif
