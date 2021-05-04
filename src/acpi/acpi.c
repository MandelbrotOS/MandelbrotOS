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

void init_acpi(struct stivale2_struct_tag_rsdp_t *rsdp) {
    struct acpi_rsdp *rsdp_ = (struct acpi_rsdp *) (uintptr_t) rsdp->rsdp;
    if (rsdp_->revision >= 2) {
        use_xsdt = true;
        xsdt = (struct acpi_xsdt *) (uintptr_t) rsdp_->xsdt_address;
    } else {
        use_xsdt = false;
        rsdt = (struct acpi_rsdt *) (uintptr_t) rsdp_->rsdt_address;
    }
}

void *acpi_get_table(const char *signature) {
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
        if (!strncmp(table_header->signature, signature, 4)) {
            return (void *) table_header;
        }
    }
    printf("acpi: warning: table with signature %s not found\n", signature);
    return NULL;
}
