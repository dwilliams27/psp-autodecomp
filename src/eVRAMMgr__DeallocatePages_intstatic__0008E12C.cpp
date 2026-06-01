// Split-TU local redeclaration for eVRAMMgr::DeallocatePages.
// DeallocatePages and UnlinkAlloc are not declared in include/eVRAMMgr.h; adding
// them there is banned. Declare the class locally and define the method out-of-line.

#pragma control sched=2

class eVRAMMgr {
public:
    struct eAllocTag {
        int           start;  // 0x00
        int           end;    // 0x04
        unsigned char flag8;  // 0x08
        char          _pad9[3];  // 0x09
        eAllocTag *prev;      // 0x0C
        eAllocTag *next;      // 0x10
    };

    static void DeallocatePages(int page);
    static void UnlinkAlloc(eAllocTag *node);
};

extern eVRAMMgr::eAllocTag *s_vramAllocHead;
extern int                  s_vramAllocFree;

void eVRAMMgr::DeallocatePages(int page) {
    eAllocTag *node;

    if (page < 0) return;

    node = s_vramAllocHead;
    while (node) {
        if (node->start == page) {
            int size = node->end - node->start;
            if (node->prev && node->prev->flag8) {
                eAllocTag *p = node->prev;
                int ps = p->start;
                __asm__ volatile("" ::: "memory");
                node->start = ps;
                eVRAMMgr::UnlinkAlloc(p);
            }
            if (node->next && node->next->flag8) {
                eAllocTag *n = node->next;
                int ne = n->end;
                __asm__ volatile("" ::: "memory");
                node->end = ne;
                eVRAMMgr::UnlinkAlloc(n);
            }
            s_vramAllocFree -= size;
            __asm__ volatile("" ::: "memory");
            node->flag8 = 1;
            return;
        }
        node = node->next;
    }
}
