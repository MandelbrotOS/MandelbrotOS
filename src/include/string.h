#ifndef __STRING_H__
#define __STRING_H__

#define DICT_LEN 256

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void *memset(void *b, int c, int len);
unsigned int strlen(const char *s);
char *strcat(char *s1, const char *s2);
char *strcpy(char *destination, const char *source);
char *itoa(int value, char *str, int base);
int atoi(char *str);
char *dyncat(char *s1, char *s2);
int strcmp(char input[], char check[]);
char *strtok(char *str, char *delim);
int tolower(int ch);
float atof(const char *s);
int isdigit(int c);
int wspaceamount(char *a);
void memcpy(void *dest, void *src, size_t n);

#endif // !__STRING_H__