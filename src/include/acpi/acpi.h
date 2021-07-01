#ifndef __ACPI_H__
#define __ACPI_H__
#include <acpi/tables.h>
#include <boot/stivale2.h>

extern rsdp_t *rsdp;
extern rsdt_t *rsdt;
extern xsdt_t *xsdt;

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info);

#endif
