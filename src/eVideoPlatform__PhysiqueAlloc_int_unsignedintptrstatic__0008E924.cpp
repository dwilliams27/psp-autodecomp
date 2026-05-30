// eVideoPlatform::PhysiqueAlloc(int, unsigned int *) static  @ 0x0008e924
// Symbol: __0fOeVideoPlatformNPhysiqueAllociPUiT
// Split-TU: declare class locally; do not touch shared headers.

class eVideoPlatform {
public:
    static int PhysiqueAlloc(int, unsigned int *);
};

// GE display-list ring / allocator block at 0x984D0.
// Sibling eVideoPlatform::Flip/Flush @0x8e2d0/0x8e5a8 use the same global.
struct eGeAlloc {
    int   slot[2];   // +0, +4 : base slots, indexed by index
    int  *unk8;      // +8     : current write pointer
    int   unkC;      // +12
    int   index;     // +16
};

#define ALLOC (*(eGeAlloc *)0x984D0)

extern "C" int sceKernelDcacheWritebackInvalidateRange(const void *start, unsigned int size);

#pragma control sched=1
int eVideoPlatform::PhysiqueAlloc(int arg0, unsigned int *out) {
    int limit = (*(int **)0x37D0C8)[4];
    __asm__ volatile("" ::: "memory");   // pin eval order: 0x37D0C8 read before 0x37D380
    int *base = (int *)0x380000;         // lui base for 0x37D380 global
    if (limit < base[(0x37D380 - 0x380000) / 4] + arg0) {
        *out = 0;
        return 0;
    }

    int words = (int)((unsigned int)((arg0 << 4) + 3) >> 2) + 3;
    __asm__ volatile("" ::: "memory");   // block cross-BB scheduling into the alloc body

    int n2 = words + 2;
    eGeAlloc *a = &ALLOC;
    int *p = a->unk8;
    int *body = p + 2;                 // (char*)p + 8
    int *next = p + n2;
    a->unk8 = next;

    p[0] = (int)((((unsigned int)next >> 0x18) & 0xf) << 0x10) | 0x10000000;
    p[1] = (int)(((unsigned int)a->unk8 & 0xffffff) | 0x08000000);

    if (body == 0) {
        *out = 0;
        return 0;
    }

    int start = (int)(((unsigned int)((int)body + 0xf) >> 4) << 4);
    __asm__ volatile("" ::: "memory");   // delta rounding must precede 0x37D124 read

    int delta = start - a->slot[a->index];
    int rounded = (delta + (int)((unsigned int)(delta >> 4) >> 0x1c)) >> 4;
    *out = (unsigned int)(((*(int *)0x37D124 & 0xffff) << 0x10) | (rounded & 0xffff));

    sceKernelDcacheWritebackInvalidateRange((const void *)start, (unsigned int)(words << 2));
    base[(0x37D380 - 0x380000) / 4] += arg0;
    return start;
}
