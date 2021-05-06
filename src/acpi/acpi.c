#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <printf.h>
#include <acpi/acpi.h>
#include <boot/stivale2.h>
#include <mm/vmm.h>
#include <lai/helpers/sci.h>

static bool use_xsdt;
static struct acpi_rsdt *rsdt;
static struct acpi_xsdt *xsdt;

uint8_t rsdp_revision;

void init_acpi(struct stivale2_struct_tag_rsdp_t *rsdp) {
  struct acpi_rsdp *rsdp_ = (struct acpi_rsdp *) (uintptr_t) (rsdp->rsdp + PHYS_MEM_OFFSET);
  rsdp_revision = rsdp_->revision;
  if (rsdp_revision >= 1) {
    use_xsdt = true;
    xsdt = (struct acpi_xsdt *) (uintptr_t) (rsdp_->xsdt_address + PHYS_MEM_OFFSET);
  } else {
    use_xsdt = false;
    rsdt = (struct acpi_rsdt *) (uintptr_t) (rsdp_->rsdt_address + PHYS_MEM_OFFSET);
  }
}

void *acpi_get_table(const char *signature, size_t index) {
  size_t entries;
  if (use_xsdt) {
    entries = (xsdt->header.length - sizeof(struct acpi_xsdt)) / sizeof(uint64_t);
  } else {
    entries = (rsdt->header.length - sizeof(struct acpi_rsdt)) / sizeof(uint32_t);
  }
  // DSDT and FACS are not found in the RSDT or XSDT
  if (!strncmp(signature, "DSDT", 4)) {
    struct acpi_fadt *fadt = (struct acpi_fadt *) acpi_get_table("FACP", 0);
    if (fadt->x_dsdt && rsdp_revision > 0) {
      return (void *) (uintptr_t) (fadt->x_dsdt + PHYS_MEM_OFFSET);
    }
    return (void *) (uintptr_t) (fadt->dsdt + PHYS_MEM_OFFSET);
  }
  if (!strncmp(signature, "FACS", 4)) {
    struct acpi_fadt *fadt = (struct acpi_fadt *) acpi_get_table("FACP", 0);
    if (fadt->x_firmware_ctrl && rsdp_revision > 0) {
      return (void *) (uintptr_t) (fadt->x_firmware_ctrl + PHYS_MEM_OFFSET);
    }
    return (void *) (uintptr_t) (fadt->firmware_ctrl + PHYS_MEM_OFFSET);
  }
  for (size_t i = 0; i < entries; i++) {
    struct acpi_sdt *table_header;
    if (use_xsdt) {
      table_header = (struct acpi_sdt *) (uintptr_t) (xsdt->entries[i] + PHYS_MEM_OFFSET);
    } else {
        table_header = (struct acpi_sdt *) (uintptr_t) (rsdt->entries[i] + PHYS_MEM_OFFSET);
    }
    if (!strncmp(table_header->signature, signature, 4) && index == 0) {
        return (void *) table_header;
    } else if (!strncmp(table_header->signature, signature, 4) && i == index) {
      return (void *) table_header;
    }
  }
  printf("acpi: warning: table with signature %s not found\r\n", signature);
  return NULL;
}

void acpi_sci_handler() {
  printf("acpi: sci %d\n\r", lai_get_sci_event());
}
