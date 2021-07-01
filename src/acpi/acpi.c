#include <acpi/acpi.h>
#include <printf.h>
#include <stdbool.h>
#include <stddef.h>

rsdp_t *rsdp;
rsdt_t *rsdt;
xsdt_t *xsdt;

// TODO: ACPI 2.0+ support

bool do_acpi_checksum(sdt_t *th) {
  uint8_t sum = 0;

  for (int i = 0; i < th->length; i++)
    sum += ((char *)th)[i];

  return sum == 0;
}

void *get_acpi_table(char *signature) {
  int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

  for (int i = 0; i < entries; i++) {
    sdt_t *h = (sdt_t *)rsdt->sptr[i];
    if (h->signature[0] == signature[0] &&
	h->signature[1] == signature[1] &&
	h->signature[2] == signature[2] &&
	h->signature[3] == signature[3]) {
	
	if (do_acpi_checksum(h))
	  return (void *)h;
	else
	  return NULL;
    }
  }

  return NULL;
}

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info) {
  rsdp = (rsdp_t *)rsdp_info->rsdp;
  
  if (!rsdp->revision) {
    rsdt = (rsdt_t *)rsdp->rsdt_address;
  } else {
    xsdt = (xsdt_t *)rsdp->xsdt_address;
  }

  return 0;
}
