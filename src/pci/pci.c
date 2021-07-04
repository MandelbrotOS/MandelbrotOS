#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <pci/pci.h>
#include <printf.h>
#include <stddef.h>

static mcfg_t *mcfg;

void pci_enumerate_function(uint64_t device_address, uint8_t function) {
  uint64_t function_address = device_address + (function << 12);

  pci_device_t *pci_device = (pci_device_t *)function_address;

  if (!pci_device->device_id || pci_device->device_id == 0xFFFF) return;

  printf("%x:%x", pci_device->vendor_id, pci_device->device_id);
}

void pci_enumerate_device(uint64_t bus_address, uint64_t device) {
  uint64_t device_address = bus_address + (device << 15);

  pci_device_t *bus_device = (pci_device_t *)device_address;

  if (!bus_device->device_id || bus_device->device_id == 0xFFFF) return;

  for (uint8_t function = 0; function < 8; function++)
    pci_enumerate_function(device_address, function);
}

void pci_enumerate_bus(uint64_t base_address, uint64_t bus) {
  uint64_t bus_address = base_address + (bus << 20);

  pci_device_t *bus_device = (pci_device_t *)bus_address;

  if (!bus_device->device_id || bus_device->device_id == 0xFFFF) return;

  for (uint64_t device = 0; device < 32; device++)
    pci_enumerate_device(bus_address, device);
}

int pci_enumerate() {
  mcfg = get_table("MCFG", 0);
  
  if (mcfg == NULL) return 1;

  int entries = (mcfg->h.length - sizeof(mcfg_t)) / sizeof(mcfg_entry_t);

  for (int t = 0; t < entries; t++) {
    mcfg_entry_t *entry = (mcfg_entry_t *)((uint64_t)mcfg + sizeof(mcfg_t) + sizeof(mcfg_entry_t) * t);

    for (uint64_t bus = entry->sbus; bus < entry->ebus; bus++)
      printf("base found at -> 0x%x, %d\r\n", entry->base_address, bus);
      //pci_enumerate_bus(entry->base_address, bus);
  }

  return 0;
}
