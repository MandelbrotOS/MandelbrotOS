#include <kernel/device.h>
#include <mm/heap.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

device_t device_list[MAX_DEVICES];

void device_init(void) {
  for (int i = 0; i < MAX_DEVICES; i++)
    device_list[i].present = 0;
}

device_t *device_add(const char *name) {
  for (int i = 0; i < MAX_DEVICES; i++) {
    if (!device_list[i].present) {
      device_list[i].present = 1;
      strcpy(device_list[i].name, name);

      return &(device_list[i]);
    }
  }

  return NULL;
}

void device_remove(device_t *device) {
  device->present = 0;
}
