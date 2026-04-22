#include "eVRAMMgr.h"

#pragma control sched=1

extern eVRAMMgr::eAllocTag *s_vramAllocTail;

void eVRAMMgr::InsertAllocAfter(eAllocTag *node, eAllocTag *pos) {
    node->prev = pos;
    eAllocTag *next = pos->next;
    __asm__ volatile("" ::: "memory");
    node->next = next;
    if (node->prev) node->prev->next = node;
    if (node->next) node->next->prev = node;
    if (s_vramAllocTail == pos) s_vramAllocTail = node;
}
