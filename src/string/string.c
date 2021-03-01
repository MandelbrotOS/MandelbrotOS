#include <kernel/alloc.h>
#include <kernel/text.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void *memset(void *b, int c, int len) {
  unsigned char *p = b;
  while (len > 0) {
    *p = c;
    p++;
    len--;
  }
  return (b);
}

void memcpy(void *dest, void *src, size_t n) {
  // Typecast src and dest addresses to (char *)
  char *csrc = (char *)src;
  char *cdest = (char *)dest;

  // Copy contents of src[] to dest[]
  for (int i = 0; i < (int)n; i++)
    cdest[i] = csrc[i];
}

unsigned int strlen(const char *s) {
  unsigned int count = 0;
  while (*s != '\0') {
    count++;
    s++;
  }
  return count;
}

char *strcat(char *s1, const char *s2) {
  // Pointer should not null pointer
  if ((s1 == NULL) && (s2 == NULL))
    return NULL;
  // Create copy of s1
  char *start = s1;
  // Find the end of the destination string
  while (*start != '\0') {
    start++;
  }
  // Now append the source string characters
  // until not get null character of s2
  while (*s2 != '\0') {
    *start++ = *s2++;
  }
  // Append null character in the last
  *start = '\0';
  return s1;
}

char *strcpy(char *destination, const char *source) {
  if (destination == NULL)
    return NULL;

  char *ptr = destination;

  while (*source != '\0') {
    *destination = *source;
    destination++;
    source++;
  }

  *destination = '\0';

  return ptr;
}

int atoi(char *str) {
  int res = 0;

  for (int i = 0; str[i] != '\0'; ++i) {
    res = res * 10 + str[i] - '0';
  }

  return res;
}

char *itoa(int value, char *str, int base) {
  char *rc;
  char *ptr;
  char *low;
  // Check for supported base.
  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }
  rc = ptr = str;
  // Set '-' for negative decimals.
  if (value < 0 && base == 10) {
    *ptr++ = '-';
  }
  // Remember where the numbers start.
  low = ptr;
  // The actual conversion.
  do {
    // Modulo is negative for negative value. This trick makes abs()
    // unnecessary.
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrst"
             "uvwxyz"[35 + value % base];
    value /= base;
  } while (value);
  // Terminating the string.
  *ptr-- = '\0';
  // Invert the numbers.
  while (low < ptr) {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  return rc;
}

char *dyncat(char *s1, char *s2) {
  char *toret, *mallocd;

  mallocd = (char *)malloc(1 + strlen(s1) + strlen(s2));

  strcpy(mallocd, s1);
  strcat(mallocd, s2);

  toret = mallocd;

  free(mallocd);

  return toret;
}

int strcmp(char input[], char check[]) {
  int i, result = 1;
  for (i = 0; input[i] != '\0' || check[i] != '\0'; i++) {
    if (input[i] != check[i]) {
      result = 0;
      break;
    }
  }
  return result;
}

int *create_delim_dict(char *delim) {
  int *d = (int *)malloc(sizeof(int) * DICT_LEN);
  memset((void *)d, 0, sizeof(int) * DICT_LEN);

  int i;
  for (i = 0; i < strlen(delim); i++) {
    d[delim[i]] = 1;
  }
  return d;
}

char *strtok(char *str, char *delim) {

  static char *last, *to_free;
  int *deli_dict = create_delim_dict(delim);

  if (!deli_dict) {
    return NULL;
  }

  if (str) {
    last = (char *)malloc(strlen(str) + 1);
    if (!last) {
      free(deli_dict);
    }
    to_free = last;
    strcpy(last, str);
  }

  while (deli_dict[*last] && *last != '\0') {
    last++;
  }
  str = last;
  if (*last == '\0') {
    free(deli_dict);
    free(to_free);
    return NULL;
  }
  while (*last != '\0' && !deli_dict[*last]) {
    last++;
  }

  *last = '\0';
  last++;

  free(deli_dict);
  return str;
}

int wspaceamount(char *a) {
  int i = 0, count = 0;

  while (a[i] != '\0') {
    if (a[i] == ' ') {
      count++;
    }
    i++;
  }

  return count;
}

int isdigit(int c) {
  if (c >= '0' && c <= '9')
    return c;
  else
    return 0;
}

float atof(const char *s) {
  float rez = 0, fact = 1;
  if (*s == '-') {
    s++;
    fact = -1;
  }
  for (int point_seen = 0; *s; s++) {
    if (*s == '.') {
      point_seen = 1;
      continue;
    }
    int d = *s - '0';
    if (d >= 0 && d <= 9) {
      if (point_seen)
        fact /= 10.0f;
      rez = rez * 10.0f + (float)d;
    }
  }
  return rez * fact;
}

int tolower(int ch) {
  if (ch >= 'A' && ch <= 'Z')
    return ('a' + ch - 'A');
  else
    return ch;
}