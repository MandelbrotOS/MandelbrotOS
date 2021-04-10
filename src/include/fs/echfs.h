#ifndef __ECHFS_H__
#define __ECHFS_H__

#include <kernel/device.h>
#include <stdint.h>

#define ECHFS_READ_MASK (0b0000000100100100)
#define ECHFS_WRITE_MASK (0b0000000010010010)
#define ECHFS_EXEC_MASK (0b0000000001001001)

#define ECHFS_ROOT_DIR_ID (0xFFFFFFFFFFFFFFFF)

#define ECHFS_TYPE_NULL (0xFF)

typedef struct echfs_table_t echfs_table_t;
typedef struct echfs_entry_t echfs_entry_t;

typedef struct echfs_t echfs_t;

struct echfs_table_t {
  uint8_t jump_code[4];
  char signature[8];

  uint64_t block_cnt;
  uint64_t dir_block_cnt;
  uint64_t block_size;

  uint32_t reserved;
  uint64_t uuid[2];
} __attribute__((packed));

struct echfs_entry_t {
  uint64_t parent_id;
  uint8_t type;

  // NULL-terminated
  char name[201];

  uint64_t atime, mtime;

  uint16_t perms;
  uint16_t own_id, grp_id;

  uint64_t ctime;

  uint64_t blk_id;
  uint64_t size;
} __attribute__((packed));

struct echfs_t {
  // Directly taken from table
  uint64_t block_cnt;
  uint64_t dir_block_cnt;
  uint64_t block_size;

  // Calculated
  uint64_t alloc_offset;
  uint64_t dir_offset;
  uint64_t dir_cnt;
};

echfs_t echfs_get_fs(device_t device);
uint64_t echfs_load_block(device_t device, echfs_t fs, uint8_t *buffer,
                          uint64_t block);
echfs_entry_t echfs_find(device_t device, echfs_t fs, uint64_t dir,
                         const char *name);
int echfs_read(device_t device, echfs_t fs, echfs_entry_t file,
               uint8_t *buffer);
uint64_t echfs_get_size(device_t device, echfs_t fs, echfs_entry_t file);

#endif // !__FS_ECHFS_H__
