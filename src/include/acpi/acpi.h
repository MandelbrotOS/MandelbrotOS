#ifndef __ACPI__
#define __ACPI__

#include <stddef.h>
#include <boot/stivale2.h>

struct acpi_rsdp {
  char signature[8];
  uint8_t checksum;
  char oem_id[6];
  uint8_t revision;
  uint32_t rsdt_address;

  // acpi 2.0+
  uint32_t length;
  uint64_t xsdt_address;
  uint8_t extended_checksum;
  char reserved[3];
} __attribute__((__packed__));

struct acpi_sdt {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oem_id[6];
  char oem_table_id[8];
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__((__packed__));

struct acpi_rsdt {
  struct acpi_sdt header;
  uint32_t entries[];
} __attribute__((__packed__));

struct acpi_xsdt {
  struct acpi_sdt header;
  uint64_t entries[];
} __attribute__((__packed__));

struct acpi_mcfg_entry {
  uint64_t base;
  uint16_t segment_number;
  uint8_t start_pci_bus;
  uint8_t end_pci_bus;
  uint32_t reserved;
} __attribute__((__packed__));

struct acpi_mcfg {
  struct acpi_sdt header;
  uint64_t reserved;
  struct acpi_mcfg_entry entries[];
} __attribute__((__packed__));

void init_acpi(struct stivale2_struct_tag_rsdp_t *rsdp);
void *acpi_get_table(const char *signature, size_t index); // index == -1: any
void *acpi_mcfg_get_seg_addr(uint16_t segment);
void lai_init(int revision);

#endif // !__ACPI__