#ifndef __ACPI_H__
#define __ACPI_H__

#include <acpi/tables.h>
#include <boot/stivale2.h>
#include <stddef.h>

extern rsdp_t *rsdp;
extern rsdt_t *rsdt;
extern xsdt_t *xsdt;
extern madt_t *madt;
extern mcfg_t *mcfg;
extern sdt_t *facp;

extern madt_ioapic_t **madt_ioapics;
extern madt_lapic_t **madt_lapics;
extern madt_nmi_t **madt_nmis;
extern madt_iso_t **madt_isos;

extern size_t ioapic_len;
extern size_t lapic_len;
extern size_t nmi_len;
extern size_t iso_len;

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info);
void *get_table(char *signature, int index);

#endif
