#include "eVRAMMgr.h"

class eVRAMMgr::eAllocTag {
public:
    char _pad0[12];
    eAllocTag *prev;  // 0x0C
    eAllocTag *next;  // 0x10
};

extern eVRAMMgr::eAllocTag *g_firstAlloc;

void eVRAMMgr::InsertAllocBefore(eAllocTag *tag, eAllocTag *before) {
    tag->prev = before->prev;
    __asm__ volatile("" ::: "memory");
    tag->next = before;
    if (tag->prev) tag->prev->next = tag;
    if (tag->next) tag->next->prev = tag;
    if (g_firstAlloc == before) g_firstAlloc = tag;
}
