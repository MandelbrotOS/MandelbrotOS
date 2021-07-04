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

typedef struct madt {
  sdt_t header;
  uint32_t local_controller_address;
  uint32_t flags;
  uint8_t entries[0];
} __attribute__((packed)) madt_t;

typedef struct madt_header {
  uint8_t id;
  uint8_t length;
} __attribute__((packed)) madt_header_t;

typedef struct madt_lapic {
  madt_header_t header;
  uint8_t processor_id;
  uint8_t apic_id;
  uint32_t flags;
} __attribute__((packed)) madt_lapic_t;

typedef struct madt_ioapic {
  madt_header_t header;
  uint8_t apic_id;
  uint8_t reserved;
  uint32_t address;
  uint32_t gsib;
} __attribute__((packed)) madt_ioapic_t;

typedef struct madt_iso {
  madt_header_t header;
  uint8_t bus_source;
  uint8_t irq_source;
  uint16_t flags;
  uint32_t gsi;
} __attribute__((packed)) madt_iso_t;

typedef struct madt_nmi {
  madt_header_t header;
  uint8_t processor;
  uint8_t lint;
  uint16_t flags;
} __attribute__((packed)) madt_nmi_t;

#endif
