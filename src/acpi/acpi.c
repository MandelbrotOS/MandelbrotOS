#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <printf.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static rsdp_t *rsdp;
static rsdt_t *rsdt;
static xsdt_t *xsdt;

bool do_acpi_checksum(sdt_t *th) {
  uint8_t sum = 0;

  for (uint32_t i = 0; i < th->length; i++)
    sum += ((char *)th)[i];

  return sum == 0;
}

void *get_table(char *signature, int index) {
  int entries;
  sdt_t *h;

  if (!rsdp->revision)
    entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;
  else
    entries = (xsdt->h.length - sizeof(xsdt->h)) / 8;

  int i = 0;
  for (int t = 0; t < entries; t++) {
    
    if (!rsdp->revision)
      h = (sdt_t *)(uint64_t)rsdt->sptr[t];
    else
      h = (sdt_t *)xsdt->sptr[i];
    
    if (!strncmp(signature, h->signature, 4)) {
      if (do_acpi_checksum(h) && i == index)
	return (void *)h;
      else
	return NULL;

      i++;
    }
  }

  return NULL;
}

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info) {
  rsdp = (rsdp_t *)rsdp_info->rsdp;
  
  if (!rsdp->revision)
    rsdt = (rsdt_t *)(uint64_t)rsdp->rsdt_address;
  else
    xsdt = (xsdt_t *)rsdp->xsdt_address;

  return 0;
}
