#ifndef __STRING_H__
#define __STRING_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void *memset(void *b, int c, size_t len);
unsigned int strlen(const char *s);
char *strcat(char *s1, const char *s2);
char *strcpy(char *destination, const char *source);
char *itoa(int value, char *str, size_t base);
int atoi(char *str);
int strcmp(const char *input, const char *check);
char tolower(char ch);
float atof(const char *s);
int isdigit(int c);
int wspaceamount(char *a);
void *memcpy(void *dest, const void *src, size_t n);

uint64_t str_to_u64(const char *str);

int strncmp(const char *s1, const char *s2, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

#endif // !__STRING_H__
