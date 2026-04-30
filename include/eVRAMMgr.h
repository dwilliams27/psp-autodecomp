#ifndef EVRAMMGR_H
#define EVRAMMGR_H

extern "C" void free(void *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eHeightmapTemplateDataDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eVRAMMgr {
public:
    struct eAllocTag {
        char _pad0[0xC];      // 0x00
        eAllocTag *prev;      // 0x0C
        eAllocTag *next;      // 0x10
    };

    static void EndRender(void);
    static void InvalidateAll(void);
    static void InsertAllocBefore(eAllocTag *node, eAllocTag *pos);
    static void InsertAllocAfter(eAllocTag *node, eAllocTag *pos);
};

class eHeightmapTemplateData {
public:
    eHeightmapTemplateData(void);
    ~eHeightmapTemplateData(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            register volatile eHeightmapTemplateDataDeleteRecord *rec __asm__("a1") =
                (volatile eHeightmapTemplateDataDeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
};

#endif
