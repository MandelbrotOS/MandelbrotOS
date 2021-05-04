#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <printf.h>
#include <acpi/acpi.h>
#include <boot/stivale2.h>

static bool use_xsdt;
static struct acpi_rsdt *rsdt;
static struct acpi_xsdt *xsdt;

static struct acpi_mcfg *mcfg;

void init_acpi(struct stivale2_struct_tag_rsdp_t *rsdp) {
  struct acpi_rsdp *rsdp_ = (struct acpi_rsdp *) (uintptr_t) rsdp->rsdp;
  if (rsdp_->revision >= 2) {
    use_xsdt = true;
    xsdt = (struct acpi_xsdt *) (uintptr_t) rsdp_->xsdt_address;
  } else {
    use_xsdt = false;
    rsdt = (struct acpi_rsdt *) (uintptr_t) rsdp_->rsdt_address;
  }
  // get mcfg
  mcfg = acpi_get_table("MCFG", -1);
  if (!mcfg) {
    printf("acpi: error: mcfg table not found. if using qemu, did you set the -machine q35 flag?\nHalting.");
    asm volatile("1:\n\tcli\n\thlt\n\tjmp 1b");
  }
}

void *acpi_get_table(const char *signature, size_t index) {
  size_t entries;
  if (use_xsdt) {
      entries = (xsdt->header.length - sizeof(struct acpi_xsdt)) / sizeof(uint64_t);
  } else {
      entries = (rsdt->header.length - sizeof(struct acpi_rsdt)) / sizeof(uint32_t);
  }
  for (size_t i = 0; i < entries; i++) {
    struct acpi_sdt *table_header;
    if (use_xsdt) {
      table_header = (struct acpi_sdt *) (uintptr_t) xsdt->entries[i];
    } else {
        table_header = (struct acpi_sdt *) (uintptr_t) rsdt->entries[i];
    }
    if (!strncmp(table_header->signature, signature, 4) && index == -1) {
        return (void *) table_header;
    } else if (!strncmp(table_header->signature, signature, 4) && i == index) {
      return (void *) table_header;
    }
  }
  printf("acpi: warning: table with signature %s not found\n", signature);
  return NULL;
}

void *acpi_mcfg_get_seg_addr(uint16_t segment) {
  size_t entries = (mcfg->header.length - sizeof(struct acpi_mcfg)) / sizeof(struct acpi_mcfg_entry);
  for (size_t i = 0; i < entries; i++) {
    if (mcfg->entries[i].segment_number == segment) {
      return (void *) (mcfg->entries[i].base + 0xffff800000000000);
    }
  }
  return NULL;
}