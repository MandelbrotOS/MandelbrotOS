#ifndef __ACPI_H__
#define __ACPI_H__

#include <boot/stivale2.h>
#include <acpi/tables.h>

extern rsdp_t *rsdp;
extern rsdt_t *rsdt;
extern xsdt_t *xsdt;
extern madt_t *madt;

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info);
void *get_table(char *signature, int index);

#endif
