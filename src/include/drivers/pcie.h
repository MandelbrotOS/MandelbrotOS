#ifndef __PCIE_H__
#define __PCIE_H__

#include <stdint.h>

// init_acpi() required before this
void init_pcie();

uint8_t pcie_readb(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);
uint16_t pcie_readw(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);
uint32_t pcie_readd(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);

void pcie_writeb(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint8_t value);
void pcie_writew(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint16_t value);
void pcie_writed(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint32_t value);

#endif
