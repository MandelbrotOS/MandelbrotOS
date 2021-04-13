#ifndef __STRING_H__
#define __STRING_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void* memset(void* b, int c, int len);
unsigned int strlen(const char* s);
char* strcat(char* s1, const char* s2);
char* strcpy(char* destination, const char* source);
char* itoa(int value, char* str, int base);
int atoi(char* str);
int strcmp(const char* input, const char* check);
char tolower(char ch);
float atof(const char* s);
int isdigit(int c);
int wspaceamount(char* a);
void memcpy(void* dest, void* src, size_t n);

uint64_t str_to_u64(const char* str);

#endif // !__STRING_H__
