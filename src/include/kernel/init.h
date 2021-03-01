#ifndef __INIT_H__
#define __INIT_H__

#include <multiboot.h>
#include <stdbool.h>

extern int inited_funs_no;

extern const char *inited_funcs[50];

void init_check(int func, char *name, bool ness);

#endif // !__INIT_H__
