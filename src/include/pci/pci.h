#ifndef __PCI_H__
#define __PCI_H__

#include <stdint.h>

typedef struct pci_device {
  uint8_t bus;
  uint8_t device;
  uint8_t function;
  uint8_t class;
  uint8_t subclass;
  uint16_t device_id;
  uint8_t vendor_id;
} pci_t;

void pci_legacy_enumerate();
int pci_enumerate();

#endif
