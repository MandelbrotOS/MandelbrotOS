#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

int isdigit(char c);
void strcpy(char *dest, const char *source);
void strcat(char *dest, const char *src);
void memcpy(void *dest, void *src, size_t n);
void memset(void *s, int c, unsigned int len);
size_t strlen(const char *s);

#endif
