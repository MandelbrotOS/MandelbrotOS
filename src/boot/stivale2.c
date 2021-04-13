#include <boot/stivale2.h>
#include <stddef.h>
#include <stdint.h>

// Stivale get tag
struct stivale2_tag_t* stivale2_get_tag(
    struct stivale2_struct_t* bootloader_info, uint64_t tag_id)
{
    struct stivale2_tag_t* tag = (struct stivale2_tag_t*)bootloader_info->tags;
    while (tag) {
        if (tag->identifier == tag_id) {
            return tag;
        }
        tag = (struct stivale2_tag_t*)tag->next;
    }
    return NULL;
}
