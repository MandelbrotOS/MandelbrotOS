#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <klog.h>
#include <mm/pmm.h>
#include <pci/pci.h>
#include <printf.h>

void pci_enumerate_fn(uint64_t device_address, uint64_t bus, uint8_t device,
                      uint8_t function) {
  uint64_t function_address = device_address + (function << 12);

  pci_device_t *pci_fn = (pci_device_t *)function_address;

  if (!pci_fn->device_id || pci_fn->device_id == 0xFFFF)
    return;

  klog(3, "%x:%x.%x -> %s, %s, %s, %s\r\n", (uint32_t)bus, device, function,
       pci_get_vendor_name(pci_fn->vendor_id),
       pci_device_classes[pci_fn->class],
       pci_get_subclass_name(pci_fn->class, pci_fn->subclass),
       pci_get_device_name(pci_fn->vendor_id, pci_fn->device_id));
}

void pci_enumerate_dev(uint64_t bus_address, uint64_t bus, uint8_t device) {
  uint64_t device_address = bus_address + (device << 15);

  pci_device_t *pci_device = (pci_device_t *)device_address;

  if (!pci_device->device_id || pci_device->device_id == 0xFFFF)
    return;

  for (uint8_t function = 0; function < 8; function++)
    pci_enumerate_fn(device_address, bus, device, function);
}

void pci_enumerate_bus(uint64_t base_address, uint64_t bus) {
  uint64_t bus_address = base_address + (bus << 20) + PHYS_MEM_OFFSET;

  pci_device_t *pci_bus = (pci_device_t *)bus_address;

  if (!pci_bus->device_id || pci_bus->device_id == 0xFFFF)
    return;

  for (uint8_t device = 0; device < 32; device++)
    pci_enumerate_dev(bus_address, bus, device);
}

int pci_enumerate() {
  mcfg_t *mcfg = (mcfg_t *)get_table("MCFG", 0);

  if (mcfg == NULL)
    return 1;

  int entries = (mcfg->h.length - sizeof(mcfg_t)) / sizeof(mcfg_entry_t);

  for (int t = 0; t < entries; t++)
    for (int bus = mcfg->entries[t].sbus; bus < mcfg->entries[t].ebus; bus++)
      pci_enumerate_bus(mcfg->entries[t].base_address, bus);

  return 0;
}
