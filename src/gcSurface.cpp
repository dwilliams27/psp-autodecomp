#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class eSurface : public cObject {
public:
    eSurface(cBase *);
    void Write(cFile &) const;
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcSurface : public eSurface {
public:
    gcSurface(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A18;
extern cType *D_0009F570;
extern char gcSurfacevirtualtable[];
extern "C" void gcEvent_ctor(void *, cBase *, const char *) asm("__0oHgcEventctP6FcBasePCc");

// 0x00137fc8
gcSurface::gcSurface(cBase *parent) : eSurface(parent) {
    *(void **)((char *)this + 4) = gcSurfacevirtualtable;
    gcEvent_ctor((char *)this + 0x6C, (cBase *)this, (const char *)0x36E304);
    gcEvent_ctor((char *)this + 0x88, (cBase *)this, (const char *)0x36E310);
    gcEvent_ctor((char *)this + 0xA4, (cBase *)this, (const char *)0x36E324);
    gcEvent_ctor((char *)this + 0xC0, (cBase *)this, (const char *)0x36E334);
    gcEvent_ctor((char *)this + 0xDC, (cBase *)this, (const char *)0x36E348);
}

// 0x00289f4c
void gcSurface::AssignCopy(const cBase *base) {
    gcSurface *other = dcast<gcSurface>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    {
        char *src = (char *)other + 0x44;
        __asm__ volatile("" : "+r"(src));
        int value = *(int *)src;
        char *dst = (char *)this + 0x44;
        __asm__ volatile("" : "+r"(dst));
        *(int *)dst = value;
        value = *(int *)((char *)other + 0x48);
        char *src4c = (char *)other + 0x4C;
        __asm__ volatile("" : "+r"(src4c) :: "memory");
        *(int *)((char *)this + 0x48) = value;
        value = *(int *)src4c;
        dst = (char *)this + 0x4C;
        __asm__ volatile("" : "+r"(dst));
        char *src50 = (char *)other + 0x50;
        __asm__ volatile("" : "+r"(src50));
        *(int *)dst = value;
        value = *(int *)src50;
        register char *dst50 __asm__("$4") = (char *)this + 0x50;
        __asm__ volatile("" : "+r"(dst50));
        *(int *)dst50 = value;
    }
    *(float *)((char *)this + 0x54) = *(float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)other + 0x58);
    *(unsigned char *)((char *)this + 0x5C) = *(unsigned char *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)other + 0x64);
    {
        char *src = (char *)other + 0x68;
        __asm__ volatile("" : "+r"(src));
        int value = *(int *)src;
        char *dst = (char *)this + 0x68;
        __asm__ volatile("" : "+r"(dst));
        *(int *)dst = value;
    }
    ((gcEvent *)((char *)this + 0x6C))->operator=(*(const gcEvent *)((char *)other + 0x6C));
    ((gcEvent *)((char *)this + 0x88))->operator=(*(const gcEvent *)((char *)other + 0x88));
    ((gcEvent *)((char *)this + 0xA4))->operator=(*(const gcEvent *)((char *)other + 0xA4));
    ((gcEvent *)((char *)this + 0xC0))->operator=(*(const gcEvent *)((char *)other + 0xC0));
    ((gcEvent *)((char *)this + 0xDC))->operator=(*(const gcEvent *)((char *)other + 0xDC));
}

// 0x0028a038
cBase *gcSurface::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcSurface *result = 0;
    gcSurface *obj = (gcSurface *)entry->fn(base, 0xF8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcSurface(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x0028a0b4
const cType *gcSurface::GetType(void) const {
    if (D_0009F570 == 0) {
        if (D_00046A18 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_00046A18 = cType::InitializeType(0, 0, 0x39, D_000385E4,
                                               0, (const char *)0x36DA4C,
                                               (const char *)0x36DA58, 5);
        }
        D_0009F570 = cType::InitializeType(0, 0, 0x7E, D_00046A18,
                                           &gcSurface::New, 0, 0, 0);
    }
    return D_0009F570;
}

// 0x00137ce4
void gcSurface::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eSurface *)this)->Write(file);

    {
        char *typePtr = *(char **)((char *)this + 0x70);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x6C;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0x8C);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x88;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0xA8);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xA4;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0xC4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xC0;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0xE0);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xDC;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    wb.End();
}
