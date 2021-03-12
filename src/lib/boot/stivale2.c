#include <stddef.h>
#include <stdint.h>
#include <boot/stivale2.h>

struct stivale2_tag *stivale2_get_tag(struct stivale2_struct *bootloader_info, uint64_t tag_id) {
    struct stivale2_tag *tag = (struct stivale2_tag *) bootloader_info->tags;
    while (tag) {
        if (tag->identifier == tag_id) {
            return tag;
        }
        tag = (struct stivale2_tag *) tag->next;
    }
    return NULL;
}
