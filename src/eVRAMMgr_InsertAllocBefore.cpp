#include "eVRAMMgr.h"

#pragma control sched=1

extern eVRAMMgr::eAllocTag *s_vramAllocHead;

void eVRAMMgr::InsertAllocBefore(eAllocTag *node, eAllocTag *pos) {
    node->prev = pos->prev;
    __asm__ volatile("" ::: "memory");
    node->next = pos;
    if (node->prev) node->prev->next = node;
    if (node->next) node->next->prev = node;
    if (s_vramAllocHead == pos) s_vramAllocHead = node;
}
