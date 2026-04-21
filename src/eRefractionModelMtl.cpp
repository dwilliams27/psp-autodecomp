#include "eTextureMap.h"

// ── Forward class declarations ──

class eColor {
public:
    unsigned int mColor;
};

class eDrawInfo;

class eModelMtl {
public:
    void ApplyCommonDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
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

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);
extern "C" void eTexCoordGen_eTexCoordGen(void *, cBase *);

extern char eRefractionModelMtlclassdesc[];

struct ClassDescEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

#pragma control sched=1

// ── ApplyDynamic ──

void eRefractionModelMtl::ApplyDynamic(const eDrawInfo &di, const mOCS &ocs, float f, unsigned int u, eColor c) const {
    ((const eModelMtl *)this)->ApplyCommonDynamic(di, ocs, f, u, c);
}

// ── Constructor ──

eRefractionModelMtl::eRefractionModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((void **)this)[0x04 / 4] = eRefractionModelMtlclassdesc;
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x74 / 4] = 1;
    ((int *)this)[0x78 / 4] = 0;
    eTexCoordGen_eTexCoordGen((char *)this + 0x7C, (cBase *)this);
    ((float *)this)[0xF0 / 4] = 0.75f;
    ((int *)this)[0xF4 / 4] = -1;
    ((int *)this)[0xF8 / 4] = 0;
    CreateData();
}

// ── PlatformRead ──

void eRefractionModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    ((unsigned char *)this)[92] |= 0x80;
    __asm__ volatile("" ::: "memory");
}

// ── Write ──

void eRefractionModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const eModelMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);
    ((cHandle *)((char *)h + 4))->Write(wb);

    void *desc = *(void **)((char *)this + 0x80);
    ClassDescEntry *entry = (ClassDescEntry *)((char *)desc + 0x28);
    short off = entry->offset;
    __asm__ volatile("" ::: "memory");
    char *tcg_base = (char *)this + 0x7C;
    void *target = tcg_base + off;
    entry->fn(target, (void *)wb._data[0]);

    wb.Write(((int *)this)[0x74 / 4]);
    wb.Write(((int *)this)[0x78 / 4]);
    wb.Write(((float *)this)[0xF0 / 4]);
    wb.Write((unsigned int)((int *)this)[0xF4 / 4]);
    wb.End();
}

// ── New ──

extern "C" void eRefractionModelMtl_eRefractionModelMtl(void *, cBase *);

eRefractionModelMtl *eRefractionModelMtl::New(cMemPool *pool, cBase *parent) {
    eRefractionModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eRefractionModelMtl *obj = (eRefractionModelMtl *)entry->fn(base, 0xFC, 4, 0, 0);
    if (obj != 0) {
        eRefractionModelMtl_eRefractionModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
