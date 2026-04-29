#include "eStandardSpriteMtl.h"

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

class eSpriteMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(float);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

template <class T> T *dcast(const cBase *);

void cFile_SetCurrentPos(void *, unsigned int);

struct WordCopy {
    int value;
};

void eStandardSpriteMtl::PlatformFree(void) {
}

void eStandardSpriteMtl::Unapply(void) const {
}

void eStandardSpriteMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const eSpriteMtl *)this)->Write(file);

    ((const cHandle *)((const char *)this + 0x48))->Write(wb);
    wb.Write(*(int *)((const char *)this + 0x64));
    wb.WriteBase(*(const cBase **)((const char *)this + 0x68));
    wb.Write(*(bool *)((const char *)this + 0x6C));
    wb.Write(*(float *)((const char *)this + 0x70));
    wb.Write(*(float *)((const char *)this + 0x74));
    wb.Write(*(float *)((const char *)this + 0x78));
    wb.Write(*(float *)((const char *)this + 0x7C));
    wb.End();
}

void eStandardSpriteMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
}

void eStandardSpriteMtl::AssignCopy(const cBase *src) {
    eStandardSpriteMtl &other = *dcast<eStandardSpriteMtl>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    int v0 = *(int *)((char *)&other + 0x5C);
    *(int *)((char *)this + 0x5C) = v0;
    *(WordCopy *)((char *)this + 0x60) =
        *(WordCopy *)((char *)&other + 0x60);
    *(int *)((char *)this + 0x64) = *(int *)((char *)&other + 0x64);
    *(int *)((char *)this + 0x68) = *(int *)((char *)&other + 0x68);
    *(unsigned char *)((char *)this + 0x6C) =
        *(unsigned char *)((char *)&other + 0x6C);
    *(float *)((char *)this + 0x70) = *(float *)((char *)&other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)&other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)&other + 0x78);
    *(float *)((char *)this + 0x7C) = *(float *)((char *)&other + 0x7C);
}

extern "C" void eStandardSpriteMtl_eStandardSpriteMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// ── eStandardSpriteMtl::New(cMemPool *, cBase *) static @ 0x001e44c4 ──
eStandardSpriteMtl *eStandardSpriteMtl::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eStandardSpriteMtl *result = 0;
    eStandardSpriteMtl *obj = (eStandardSpriteMtl *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        eStandardSpriteMtl_eStandardSpriteMtl(obj, parent);
        result = obj;
    }
    return result;
}
