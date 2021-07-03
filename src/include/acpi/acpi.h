#ifndef __ACPI_H__
#define __ACPI_H__
#include <boot/stivale2.h>

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info);
void *get_table(char *signature);

#endif
