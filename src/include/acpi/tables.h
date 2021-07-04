#ifndef __ACPI_TABLES_H__
#define __ACPI_TABLES_H__
#include <stdint.h>

typedef struct rsdp {
  char signature[8];
  uint8_t checksum;
  char oem_id[6];
  uint8_t revision;
  uint32_t rsdt_address;

  /* 2.0 */
  uint32_t length;
  uint64_t xsdt_address;
  uint8_t echecksum;
  uint8_t reserved[3];
} __attribute__((packed)) rsdp_t;

typedef struct sdt {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oem_id[6];
  char oem_table_id[8];
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__((packed)) sdt_t;

typedef struct rsdt {
  sdt_t h;
  uint32_t sptr[];
} __attribute__((packed)) rsdt_t;

typedef struct xsdt {
  sdt_t h;
  uint64_t sptr[];
} __attribute__((packed)) xsdt_t;

typedef struct mcfg_entry {
  uint64_t base_address;
  uint16_t pci_seg_grp;
  uint8_t sbus;
  uint8_t ebus;
  uint32_t reserved;
} __attribute__((packed)) mcfg_entry_t;

typedef struct mcfg {
  sdt_t h;
  uint64_t reserved;
  mcfg_entry_t entries[];
} __attribute__((packed)) mcfg_t;

#endif
