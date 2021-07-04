#ifndef __PCI_H__
#define __PCI_H__
#include <stdint.h>

typedef struct _pci_device_t {
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
        uint8_t htype;
        uint8_t bist;
} pci_device_t;

int pci_enumerate();

#endif
