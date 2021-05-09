#include <kernel/panic.h>
#include <kernel/text.h>
#include <kernel/fb.h>
#include <register.h>
#include <printf.h>
#include <halt.h>

void panic(const char *message, const char *error)
{
    init_color(0xff0000, 0x990000, 0x00ff00, 0x009900, 0xffff00, 0x999900,
             0x0000ff, 0x000099, 0xff00ff, 0x990099, 0x00ffff, 0x009999,
             0xffffff, 0x000000, 0x999999, 0x444444, RED, WHITE);
    init_text(5);
    cls();
    printf("Oh no! Looks like the system just died.\r\nBefore dying, the operating system \
got some information about the error:\r\n\r\nError message: %s\r\nError: %s\r\n", message, error);
    getregisters(true);
}
