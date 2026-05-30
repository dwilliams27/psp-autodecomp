// ODR-WARNING: split-TU local redeclaration for one eVRAMMgr method.
// UnlinkAlloc is not declared in include/eVRAMMgr.h; adding it there is banned.

#pragma control sched=1

class eVRAMMgr {
public:
    struct eAllocTag {
        char  _pad0[9];      // 0x00
        char  flag9;         // 0x09
        char  _pad1[2];      // 0x0A
        eAllocTag *prev;     // 0x0C
        eAllocTag *next;     // 0x10
    };

    static void UnlinkAlloc(eAllocTag *node);
};

extern eVRAMMgr::eAllocTag *s_vramAllocHead;
extern eVRAMMgr::eAllocTag *s_vramAllocTail;

void eVRAMMgr::UnlinkAlloc(eAllocTag *node) {
    if (node == s_vramAllocHead) s_vramAllocHead = node->next;
    if (node == s_vramAllocTail) s_vramAllocTail = node->prev;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    node->flag9 = 0;
}
