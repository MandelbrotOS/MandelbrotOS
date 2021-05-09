#include <kernel/panic.h>
#include <kernel/text.h>
#include <kernel/fb.h>
#include <register.h>
#include <printf.h>
#include <halt.h>

void panic(const char *message, const char *error)
{
    set_color(RED, WHITE);
    init_text(5);
    cls();
    printf("Oh no! Looks like the system just died.\r\nBefore dying, the operating system \
got some information about the error:\r\n\r\nError message: %s\r\nError: %s\r\n", message, error);
    getregisters(true);
}
