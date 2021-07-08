#include <asm.h>
#include <klog.h>
#include <mm/kheap.h>
#include <pci/pci.h>
#include <printf.h>

uint64_t pci_devc = 0;
pci_t *pci_devices;

uint32_t pci_legacy_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
  uint32_t target = (1 << 31) | ((uint32_t)bus << 16) |
                    		((uint32_t)(device & 31) << 11) |
                		((uint32_t)(function & 7) << 8) | (reg & 0xFC);

  outl(0xCF8, target);

  return inl(0xCFC);
}

uint16_t pci_legacy_get_vendor_id(uint8_t bus, uint8_t device, uint8_t function) {
  return (uint16_t)(pci_legacy_read_dword(bus, device, function, 0));
}

uint16_t pci_legacy_get_device_id(uint8_t bus, uint8_t device, uint8_t function) {
  return (uint16_t)(pci_legacy_read_dword(bus, device, function, 0) >> 16);
}

uint8_t pci_legacy_get_class(uint8_t bus, uint8_t device, uint8_t function) {
  return (uint8_t)(pci_legacy_read_dword(bus, device, function, 0x8) >> 24);
}

uint8_t pci_legacy_get_sclass(uint8_t bus, uint8_t device, uint8_t function) {
  return (uint8_t)(pci_legacy_read_dword(bus, device, function, 0x8) >> 16);
}

uint8_t pci_legacy_get_hdr(uint8_t bus, uint8_t device, uint8_t function) {
  return (uint8_t)(pci_legacy_read_dword(bus, device, function, 0xC) >> 16);
}

uint8_t pci_legacy_get_sbus(uint8_t bus, uint8_t device, uint8_t function) {
  return (uint8_t)(pci_legacy_read_dword(bus, device, function, 0x18) >> 8);
}

uint8_t pci_legacy_is_bridge(uint8_t bus, uint8_t device, uint8_t function) {
  if ((pci_legacy_get_hdr(bus, device, function) & ~(0x80)) != 0x1)
    return 0;

  if (pci_legacy_get_class(bus, device, function) != 0x6)
    return 0;

  if (pci_legacy_get_sclass(bus, device, function) != 0x4)
    return 0;

  return 1;
}

void pci_legacy_add_device(uint8_t bus, uint8_t device, uint8_t function) {
  pci_devices = krealloc(pci_devices, sizeof(pci_t) * (pci_devc + 1));

  pci_devices[pci_devc] = (pci_t){
      .bus = bus,
      .device = device,
      .function = function,
      .class = pci_legacy_get_class(bus, device, function),
      .subclass = pci_legacy_get_sclass(bus, device, function),
      .vendor_id = pci_legacy_get_vendor_id(bus, device, function),
      .device_id = pci_legacy_get_device_id(bus, device, function),
  };

  pci_devc++;
}

void pci_scan_fn(uint8_t bus, uint8_t device) {
  if (pci_legacy_get_hdr(bus, device, 0) & 0x80) {
    for (uint8_t function = 0; function < 8; function++)
      if (pci_legacy_get_vendor_id(bus, device, function) != 0xFFFF)
        pci_legacy_add_device(bus, device, function);
  } else
    pci_legacy_add_device(bus, device, 0);
}

void pci_legacy_scan_bus(uint8_t bus) {
  for (uint8_t device = 0; device < 32; device++) {
    if (pci_legacy_get_vendor_id(bus, device, 0) != 0xFFFF) {
      pci_scan_fn(bus, device);
    }
  }
}

void pci_legacy_enumerate() {
  pci_devices = kmalloc(0);
  pci_legacy_scan_bus(0);

  for (size_t device = 0; device < pci_devc; device++)
    klog(3, "%s / %s / %s / %s\r\n",
	pci_get_vendor_name(pci_devices[device].vendor_id),
	pci_get_subclass_name(pci_devices[device].class, pci_devices[device].subclass),
	pci_device_classes[pci_devices[device].class],
	pci_get_device_name(pci_devices[device].vendor_id, pci_devices[device].device_id));
}
