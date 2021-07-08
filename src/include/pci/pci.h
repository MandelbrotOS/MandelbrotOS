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
  uint16_t vendor_id;
} pci_t;

extern const char *pci_device_classes[];

const char *pci_get_subclass_name(uint8_t class, uint8_t subclass);
const char *pci_get_vendor_name(uint16_t vendor_id);
const char *pci_get_device_name(uint16_t vendor_id, uint16_t device_id);
void pci_legacy_enumerate();
int pci_enumerate();

#endif
