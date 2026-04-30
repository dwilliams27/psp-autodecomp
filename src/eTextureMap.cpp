#include "eTextureMap.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FE8;
extern cType *D_00046C3C;

struct eTextureMapVTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

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

// ── GetType ──

const cType *eTextureMap::GetType(void) const {
    if (D_00046C3C == 0) {
        if (D_00040FE8 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385DC;
                    cBase *(*factory)(cMemPool *, cBase *) =
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                    __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                    D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                       factory, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_000385E4;
            const char *kindName = (const char *)0x36CDA8;
            const char *kindDesc = (const char *)0x36CDB4;
            __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
            D_00040FE8 = cType::InitializeType(0, 0, 0xA, parentType,
                                               0, kindName, kindDesc, 5);
        }
        const cType *parentType = D_00040FE8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&eTextureMap::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C3C = cType::InitializeType(0, 0, 0xC, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C3C;
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

// ── AssignCopy ──

void eTextureMap::AssignCopy(const cBase *base) {
    const cBase *other = 0;
    if (base != 0) {
        if (D_00046C3C == 0) {
            if (D_00040FE8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) =
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                           factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                       0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                const char *kindName = (const char *)0x36CDA8;
                const char *kindDesc = (const char *)0x36CDB4;
                __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                D_00040FE8 = cType::InitializeType(0, 0, 0xA, parentType,
                                                   0, kindName, kindDesc, 5);
            }
            const cType *parentType = D_00040FE8;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))&eTextureMap::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C3C = cType::InitializeType(0, 0, 0xC, parentType, factory,
                                               0, 0, 0);
        }

        const cType *myType = D_00046C3C;
        void *vt = ((void **)base)[1];
        eTextureMapVTableSlot *slot = (eTextureMapVTableSlot *)((char *)vt + 8);
        void *type = (void *)slot->getType((char *)base + slot->offset);
        int ok;
        if (myType == 0) {
            goto fail;
        }
        if (type != 0) {
        loop:
            if (type != myType) {
                type = *(void **)((char *)type + 0x1C);
                if (type == 0) {
                    goto fail;
                }
                goto loop;
            }
            ok = 1;
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = base;
        }
    }
    *this = *(const eTextureMap *)other;
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
