#include "eVRAMMgr.h"

class eVRAMMgr::eAllocTag {
public:
    char _pad0[12];
    eAllocTag *prev;  // 0x0C
    eAllocTag *next;  // 0x10
};

extern eVRAMMgr::eAllocTag *g_lastAlloc;

void eVRAMMgr::InsertAllocAfter(eAllocTag *tag, eAllocTag *after) {
    tag->prev = after;
    __asm__ volatile("" ::: "memory");
    tag->next = after->next;
    __asm__ volatile("" ::: "memory");
    if (tag->prev) tag->prev->next = tag;
    if (tag->next) tag->next->prev = tag;
    if (g_lastAlloc == after) g_lastAlloc = tag;
}
