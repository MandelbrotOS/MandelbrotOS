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

// See 5.2.9 of ACPI Specification, Version 6.3,
// for more information about the fields
struct acpi_fadt {
  struct acpi_sdt header;
  uint32_t firmware_ctrl;
  uint32_t dsdt;
  uint8_t reserved;
  uint8_t preferred_pm_profile;
  uint16_t sci_int;
  uint32_t sci_cmd;
  uint8_t acpi_enable;
  uint8_t acpi_disable;
  uint8_t s4bios_req;
  uint8_t pstate_cnt;
  uint32_t pm1a_evt_blk;
  uint32_t pm1b_evt_blk;
  uint32_t pm1a_cnt_blk;
  uint32_t pm1b_cnt_blk;
  uint32_t pm2_cnt_blk;
  uint32_t pm_tmr_blk;
  uint32_t gpe0_blk;
  uint32_t gpe1_blk;
  uint8_t pm1_evt_len;
  uint8_t pm1_cnt_len;
  uint8_t pm2_cnt_len;
  uint8_t pm_tmr_len;
  uint8_t gpe0_blk_len;
  uint8_t gpe1_blk_len;
  uint8_t gpe1_base;
  uint8_t cst_cnt;
  uint16_t p_lv2_lat;
  uint16_t p_lv3_lat;
  uint16_t flush_size;
  uint16_t flush_stride;
  uint8_t duty_offset;
  uint8_t duty_width;
  uint8_t day_alrm;
  uint8_t mon_alrm;
  uint8_t century;
  uint16_t iapc_boot_arch;
  uint8_t reserved_2;
  uint32_t flags;
  uint8_t reset_reg[12];
  uint8_t reset_value;
  uint16_t arm_boot_arch;
  uint8_t fadt_minor_version;
  uint64_t x_firmware_ctrl;
  uint64_t x_dsdt;
  // there's a bit more but they're mostly extended
  // addresses
} __attribute__((__packed__));

extern uint8_t rsdp_revision;

void init_acpi(struct stivale2_struct_tag_rsdp_t *rsdp);
void *acpi_get_table(const char *signature, size_t index); // index == -1: any
void acpi_sci_handler();
void lai_init(int revision);

#endif // !__ACPI__
