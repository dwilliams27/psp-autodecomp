#include "eTextureMap.h"

// ── GetNumExternalDependencies ──

int eTextureMap::GetNumExternalDependencies(void) const {
    return ((*(short *)((char *)this + 0x1C) == 0) & 0xFF) == 0;
}

// ── Constructor ──

extern char eTextureMapclassdesc[];

eTextureMap::eTextureMap(cBase *parent) : eTexture(parent) {
    ((void **)this)[1] = eTextureMapclassdesc;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x4C] = 1;
    ((unsigned char *)this)[0x4D] = 0xFF;
    ((int *)this)[0x50 / 4] = 0;
}

// ── Write ──

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned char);
    void End(void);
};

void eTextureMap::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eTexture::Write(file);
    wb.Write(field_4C);
    wb.Write(field_4D);
    wb.End();
}

// ── operator= ──

class cObject {
public:
    cObject &operator=(const cObject &);
};

eTextureMap &eTextureMap::operator=(const eTextureMap &other) {
    ((cObject *)this)->operator=(*(const cObject *)&other);
    ((signed char *)this)[0x44] = ((const signed char *)&other)[0x44];
    ((signed char *)this)[0x45] = ((const signed char *)&other)[0x45];
    ((signed char *)this)[0x46] = ((const signed char *)&other)[0x46];
    ((unsigned char *)this)[0x47] = ((const unsigned char *)&other)[0x47];
    ((short *)this)[0x48 / 2] = ((const short *)&other)[0x48 / 2];
    ((short *)this)[0x4A / 2] = ((const short *)&other)[0x4A / 2];
    ((unsigned char *)this)[0x4C] = ((const unsigned char *)&other)[0x4C];
    ((unsigned char *)this)[0x4D] = ((const unsigned char *)&other)[0x4D];
    __asm__ volatile("" ::: "memory");
    CreateData();
    return *this;
}

// ── CreateData ──

void eTextureMap::CreateData(void) {
}

// ── New ──

void eTextureMap_eTextureMap(eTextureMap *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eTextureMap *eTextureMap::New(cMemPool *pool, cBase *parent) {
    eTextureMap *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eTextureMap *obj = (eTextureMap *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        eTextureMap_eTextureMap(obj, parent);
        result = obj;
    }
    return result;
}

// ── Other class stubs ──

void eProjectedModelMtl::PlatformFree(void) {
}

void eProjectedModelMtl::Unapply(void) const {
}

void eProjectedModelMtl::CreateData(void) {
}

void eShadowFillModelMtl::PlatformFree(void) {
}
