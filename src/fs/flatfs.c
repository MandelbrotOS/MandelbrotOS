#include <fs/flatfs.h>
#include <kernel/device.h>
#include <stdint.h>
#include <string.h>

// Returns flatfs header on device
flatfs_t flatfs_get_fs(device_t device) {
  flatfs_t fs;
  device.read(device.device, (void *)512 - sizeof(flatfs_t), (uint64_t)(&fs),
              sizeof(flatfs_t));

  return fs;
}

// Loads header from specified pointer 
flatfs_header_t flatfs_get_header(device_t device, uint64_t header_ptr) {
  flatfs_header_t header;
  if (!device.read(device.device, (void *)(header_ptr << 9),
                   (uint64_t)(&header), sizeof(flatfs_header_t)))
    header.type = FLAT_TYPE_NULL;

  return header;
}

// Saves header to specified point
int flatfs_set_header(device_t device, uint64_t header_ptr,
                      flatfs_header_t header) {
  return device.write(device.device, (void *)(header_ptr << 9),
                      (uint64_t)(&header), sizeof(flatfs_header_t));
}

// Look for name in dir on fs
uint64_t flatfs_find(device_t device, uint64_t dir, const char *name) {
  flatfs_header_t dir_header = flatfs_get_header(device, dir);
  if (dir_header.type == FLAT_TYPE_NULL)
    return 0;

  uint64_t entry_cnt = (dir_header.block_cnt << 9) / sizeof(uint64_t);

  // Load all the entries from this block
  uint64_t entries[entry_cnt];
  if (!device.read(device.device, (void *)((dir + 1) << 9),
                   (uint64_t)(&entries), dir_header.block_cnt << 9))
    return 0;

  flatfs_header_t header;

  // Compare each of the loaded entries
  for (uint64_t i = 0; i < entry_cnt; i++) {
    header = flatfs_get_header(device, entries[i]);
    if (header.type == FLAT_TYPE_NULL)
      return 0;

    if (!(header.type & FLAT_TYPE_MASK) && !strcmp(header.data.name, name))
      return entries[i];
  }

  // Ok, we didn't find it on this block, let's check the next one
  if (dir_header.next_ptr)
    return flatfs_find(device, dir_header.next_ptr, name);

  // No entry found with that name, abort
  return 0;
}

// Delete entry of name in dir on fs
int flatfs_delete(device_t device, uint64_t dir, const char *name) {
  flatfs_header_t dir_header = flatfs_get_header(device, dir);
  if (dir_header.type == FLAT_TYPE_NULL)
    return 0;

  uint64_t entry_cnt = (dir_header.block_cnt << 9) / sizeof(uint64_t);

  // Load all the entries from this block
  uint64_t entries[entry_cnt];
  if (!device.read(device.device, (void *)((dir + 1) << 9),
                   (uint64_t)(&entries), dir_header.block_cnt << 9))
    return 0;

  flatfs_header_t header;

  // Compare each of the loaded entries
  for (uint64_t i = 0; i < entry_cnt; i++) {
    header = flatfs_get_header(device, entries[i]);
    if (header.type == FLAT_TYPE_NULL)
      return 0;

    if (!(header.type & FLAT_TYPE_MASK) && !strcmp(header.data.name, name)) {
      header.type = FLAT_TYPE_FREE;
      flatfs_set_header(device, entries[i], header);

      return 1;
    }
  }

  // Ok, we didn't find it on this block, let's check the next one
  if (dir_header.next_ptr)
    return flatfs_find(device, dir_header.next_ptr, name);

  // No entry found with that name, abort
  return 0;
}

// Reads file from fs to buffer
int flatfs_read(device_t device, uint64_t header_ptr, uint8_t *buffer) {
  flatfs_header_t header;

  while (header_ptr) {
    // Load current header
    header = flatfs_get_header(device, header_ptr);
    if (header.type == FLAT_TYPE_NULL)
      return 0;

    // Read to buffer and increment it
    if (!device.read(device.device, (void *)((header_ptr + 1) << 9),
                     (uint64_t)buffer, header.block_cnt << 9))
      return 0;
    buffer += header.block_cnt << 9;

    // Prepare to load next block
    header_ptr = header.next_ptr;
  }

  return 1;
}

// Returns size of file at header
uint64_t flatfs_get_size(device_t device, uint64_t header_ptr) {
  // This one is pretty easy :)
  flatfs_header_t header = flatfs_get_header(device, header_ptr);
  if (header.type == FLAT_TYPE_NULL || header.type & FLAT_TYPE_MASK)
    return 0;

  return header.data.size;
}

