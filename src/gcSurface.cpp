#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;

class cObject {
public:
    cObject &operator=(const cObject &);
};

class eSurface : public cObject {
public:
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
