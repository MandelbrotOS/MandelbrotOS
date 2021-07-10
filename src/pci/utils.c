#include <acpi/acpi.h>
#include <mm/pmm.h>
#include <pci/pci.h>

static mcfg_entry_t *get_entry(uint16_t segment, uint8_t bus) {
  size_t entries = (mcfg->h.length - sizeof(mcfg_t)) / sizeof(mcfg_entry_t);

  for (size_t i = 0; i < entries; i++) {
    mcfg_entry_t *entry = &mcfg->entries[i];
    if (entry->seg_grp == segment && entry->sbus <= bus && entry->ebus >= bus)
      return entry;
  }

  return NULL;
}

static uintptr_t get_device_addr(mcfg_entry_t *entry, uint8_t bus,
                                 uint8_t device, uint8_t function) {
  return (entry->base_address + PHYS_MEM_OFFSET +
          (((bus - entry->sbus) << 20) | (device << 15) | (function << 12)));
}

uint8_t pci_cfg_read_byte(uint16_t segment, uint8_t bus, uint8_t device,
                          uint8_t function, uint16_t offset) {
  mcfg_entry_t *entry = get_entry(segment, bus);

  if (entry == NULL)
    return 0;

  return *(uint8_t *)(get_device_addr(entry, bus, device, function) + offset);
}

uint16_t pci_cfg_read_word(uint16_t segment, uint8_t bus, uint8_t device,
                           uint8_t function, uint16_t offset) {
  mcfg_entry_t *entry = get_entry(segment, bus);

  if (entry == NULL)
    return 0;

  return *(uint16_t *)(get_device_addr(entry, bus, device, function) + offset);
}

uint32_t pci_cfg_read_dword(uint16_t segment, uint8_t bus, uint8_t device,
                            uint8_t function, uint16_t offset) {
  mcfg_entry_t *entry = get_entry(segment, bus);

  if (entry == NULL)
    return 0;

  return *(uint32_t *)(get_device_addr(entry, bus, device, function) + offset);
}

void pci_cfg_write_byte(uint16_t segment, uint8_t bus, uint8_t device,
                        uint8_t function, uint16_t offset, uint8_t value) {
  mcfg_entry_t *entry = get_entry(segment, bus);

  if (entry == NULL)
    return;

  *(uint8_t *)(get_device_addr(entry, bus, device, function) + offset) = value;
}

void pci_cfg_write_word(uint16_t segment, uint8_t bus, uint8_t device,
                        uint8_t function, uint16_t offset, uint16_t value) {
  mcfg_entry_t *entry = get_entry(segment, bus);

  if (entry == NULL)
    return;

  *(uint16_t *)(get_device_addr(entry, bus, device, function) + offset) = value;
}

void pci_cfg_write_dword(uint16_t segment, uint8_t bus, uint8_t device,
                         uint8_t function, uint16_t offset, uint32_t value) {
  mcfg_entry_t *entry = get_entry(segment, bus);

  if (entry == NULL)
    return;

  *(uint32_t *)(get_device_addr(entry, bus, device, function) + offset) = value;
}

int pci_get_bar(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function,
                int bar, struct pci_bar *bar_info) {
  if (bar > 5 || bar < 0)
    return 1;

  uint16_t bar_offset = 0x10 + (bar * 4);
  uint32_t bar_value =
      pci_cfg_read_dword(segment, bus, device, function, bar_offset);

  // Get bar kind
  enum PCI_BAR_KIND kind = bar_value & 1;
  enum PCI_BAR_BITNESS bitness;

  if (kind == PCI_BAR_KIND_MMIO) {
    bitness = ((bar_value >> 1) & 0b11);
    if (bitness == PCI_BAR_BITNESS_16)
      return 1; // Forbidden
  }

  // Get bar size
  size_t bar_size;
  pci_cfg_write_dword(segment, bus, device, function, bar_offset, 0xffffffff);

  if (kind == PCI_BAR_KIND_IO) {
    bar_size = pci_cfg_read_dword(segment, bus, device, function, bar_offset);
    bar_size &= ~0b11;
    bar_size = ~bar_size + 1;
  } else if (kind == PCI_BAR_KIND_MMIO) {
    bar_size = pci_cfg_read_dword(segment, bus, device, function, bar_offset);
    bar_size &= ~0b1111;
    bar_size = ~bar_size + 1;
  }

  pci_cfg_write_dword(segment, bus, device, function, bar_offset, bar_value);
  if (bar_size == 0)
    return 1; // Non-existent bar

  bar_info->kind = kind;
  bar_info->size = bar_size;
  if (kind == PCI_BAR_KIND_IO) {
    bar_info->base = bar_value & ~0b11;
    return 0;
  }

  if (bitness == PCI_BAR_BITNESS_32) {
    bar_info->base = bar_value & ~0b1111;
  } else if (bitness == PCI_BAR_BITNESS_64) {
    bar_info->base = ((uint64_t)pci_cfg_read_dword(segment, bus, device,
                                                   function, bar_offset + 4)
                      << 32) |
                     (bar_value & ~0b1111);
  }

  return 0;
}
