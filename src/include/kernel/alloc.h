#ifndef __SHMALL_H__
#define __SHMALL_H__

#include <stddef.h>
#include <stdint.h>

int init_heap(uint32_t start);
void *malloc(uint32_t size);
void free(void *ptr);
void *calloc(size_t n, size_t size);
void *realloc(void *ptr, size_t len);

#endif // !__SHMALL_H__