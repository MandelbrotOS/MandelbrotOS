#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <printf.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


static rsdp_t *rsdp;
static rsdt_t *rsdt;
static xsdt_t *xsdt;

// TODO: ACPI 2.0+ support

bool do_acpi_checksum(sdt_t *th) {
  uint8_t sum = 0;

  for (uint32_t i = 0; i < th->length; i++)
    sum += ((char *)th)[i];

  return sum == 0;
}

void *get_table(char *signature)
{
  int entries;
  sdt_t *h;
  
  entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

  for (int t = 0; t < entries; t++) {
    h = (sdt_t *)(uint64_t)(rsdt->sptr[t] + KERNEL_MEM_OFFSET);

    if (!strncmp(signature, h->signature, 4))
      if (do_acpi_checksum(h))
        return (void *)h;
  }

  return NULL;
}

int init_acpi(struct stivale2_struct_tag_rsdp *rsdp_info) {
  rsdp = (rsdp_t *)rsdp_info->rsdp;

  if (!rsdp->revision)
    rsdt = (rsdt_t *)(rsdp->rsdt_address + KERNEL_MEM_OFFSET);
  else
    xsdt = (xsdt_t *)(rsdp->xsdt_address + KERNEL_MEM_OFFSET);

  sdt_t *facp = get_table("FACP");
  printf("%s\r\n", facp->signature);

  return 0;
}
