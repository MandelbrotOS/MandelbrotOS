#ifndef __KSHELL_H__
#define __KSHELL_H__

#include <multiboot.h>

#define check_cmd(c) strcmp((char *)argv[0], (char *)c)

int kshell(multiboot_info_t *mbi, unsigned long magic);

#endif // !__KSHELL_H__